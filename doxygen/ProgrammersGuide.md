# Maestro Audio Framework Programmer's Guide

## Introduction

Maestro audio framework provides instruments for playback and capture of different audio streams. In order to do that the framework uses API for creating various audio and voice pipelines with the support of media and track information.
This document describes the framework in its detail, and the usage of API for pipeline creation using different elements. The framework needs an operating system in order to create different tasks for audio processing and communication with the application.

## Architecture overview

A high-level block diagram of the streamer used in Maestro is shown below. An element is the most important class
of objects in the streamer (see `streamer_element.c`). A chain of elements will be created and linked together when a pipeline is
created. Data flows through this chain of elements in form of data buffers. An element has one specific function, which can be the reading
of data from a file, decoding of this data, or outputting this data to a sink device. By chaining together several such
elements, a pipeline is created that can do a specific task, for example, the playback.

## Pipeline

![maestro overview](common/maestro.svg)

The pipeline itself is created within the `streamer_create` API using the `streamer_create_pipeline` call. In the underlying `streamer_msg_create_pipeline` call the correct pipeline type is selected and then the specific `streamer_build_pipeline` is used.
In order to create a new user defined pipeline it is necessary to create a new source file in the pipeline folder. The new pipeline type needs to be added into the `StreamPipelineType` enum in the `streamer_api.h` file.
Then in the `streamer_msg_create_pipeline` function it is necessary to add a case for that particular pipeline in the `switch(streamer_msg->pipeline_type)` statement. The `streamer_build_new_pipeline` function needs to implement the building of the desired pipeline using following API:

- `create_pipeline`      - allocate memory for a new pipeline and initialize the maximum elements
- `create_element`       - create an element with the desired type, initialize its source and sink pads
- `add_element_pipeline` - add the created elements into the pipeline
- `link_elements`        - connect the elements together linking the source pads to sink pads and the other way round

Then it is also necessary to implement a new function for destroying the pipeline and register it in the `streamer_msg_destroy_pipeline` function, i.e. `stream_destroy_new_pipeline` using following API:

- `unlink_elements`         - disconnect the elements
- `remove_element_pipeline` - remove the elements from the pipeline
- `destroy_element`         - destroy the elements and free the memory
- `destroy_pipeline`        - destroy the pipeline and free the memory

## Elements and Pads
Pads are elements' inputs and outputs. A pad can be viewed as a "plug" or "port" on an element where links
may be made with other elements, and through which data can flow to or from those elements. Data flows out of an
element through a source pad, and elements accept incoming data through a sink pad. Source and sink elements
have only source and sink pads, respectively. For detailed information about pads, please see the API reference from `pad.c`.

![data flow](common/elements.svg)

## Internal communication
The streamer (the core of the framework) provides several mechanisms for communication and data exchange between the application, a
pipeline, and pipeline elements:
- buffers are objects for passing streaming data between elements in the pipeline. Buffers always travel from
sources to sinks (downstream).
- messages are objects sent from the application to the streamer task to construct, configure, and control a
streamer pipeline.
- callbacks are used to transmit information such as errors, tags, state changes, etc. from the pipeline and
elements to the application.
- events are objects sent between elements. Events can travel upstream and downstream. Events may also
be sent to the application
- queries allow applications to request information such as duration or current playback position from the
pipeline. Elements can also use queries to request information from their peer elements (such as the file size
or duration). They can be used both ways within a pipeline, but upstream queries are more common

## Decoders and encoders
Maestro framework uses a common codec interface for decoding purposes and a common encoder interface for encoding. Those interfaces encapsulate the usage of specific codecs.

### Common codec interface
The Common Codec Interface is the intended interface for all used **decoders**. The framework will integrate a CCI decoder
element into the streamer to interface with all decoders.

### Using the CCI to interface with Metadata
- `cci_extract_meta_data` must be called before any other Codec Interface APIs. This API extracts the
  metadata information of the codec and fills this information in the file_meta_data_t structure. The file_meta_data_t structure must be allocated by the application.
- This function first extracts the input file extension and based on that it calls the specific codec’s metadata
  extraction function. If it finds an invalid extension or unsupported extension then it returns
  with META_DATA_FILE_NOT_SUPPORTED code for any unsupported file format.
- If this API finds the valid metadata then it returns with META_DATA_FOUND code. If this API does not
  find any metadata information then it returns with META_DATA_NOT_FOUND code. It also returns
  with META_DATA_FILE_NOT_SUPPORTED code for any unsupported file format.

### Using the CCI to interface with Decoders
- `codec_get_mem_info` gets the memory requirement based on the specific decoder stream type. It returns
the size in bytes of the specific codec. The user of the decoders must allocate memory of this size and this
memory is used by the initialization API. The user or application must pass this allocated memory pointer to
the init API.
- `codec_init` must be called before the codec’s decode API. This API calls the codec-specific initialization function
based on the codec stream type. This API allocates the memory to the codec main structure and also initializes
the codec main structure parameters. It also registers the call back functions to the codec which will be used
by the codec to read or seek the input stream.
- `codec_decode` is the main decoding API of the codec. This API calls the codec-specific decoding function based
on the codec stream type. This API decodes the input raw stream and fills the PCM output samples into
codec output PCM buffer. This API gives the information about the number of samples produced by the codec
and also gives the pointer of the codec output PCM samples buffer.
- `codec_get_pcm_samples` must be called after the codec’s decode API. This API calls the codec specific
Get PCM Sample API based on the codec stream type. This API gets the PCM samples from the codec in
constant block size and fills them into the output PCM buffer. It returns the number of samples get from the codec
and also gives the pointer of the output PCM buffer.
- `codec_reset` calls the codec specific reset API base on stream type and resets the codec.
- `codec_seek` accepts the seek bytes offset converted from the time by application. This API calls the decoder’s
internal seek API to calculate the actual seek offset which frame boundary aligns. This API returns the
actual seek offset.

The basic sequence to use a decoder with the CCI is shown below:

![cci_decoder_flow](common/codec_flow.png)

### Adding new decoders to the CCI
This section explains how to integrate a new decoder in the Common Codec Interface. The CCI assumes the decoder
library to be used is in the `libs` folder of the maestro framework. The CCI is just a wrapper around a specific
implementation. The decoder is expected to be extended as needed to meet the APIs described above.
- Register Decoder Top level APIs in Common Codec Interface
	- Place the decoder lib in `libs` folder.
	- Add prototypes of the decoder top level APIs in codec_interface.h file (located at streamer/cci/include
	folder).
	- In codec_interface.c file (located at streamer/decoders/cci_dec), add top level Decoder APIs in decoder function
	table.
	- Pseudo code for this is as described below.

```c
const codec_interface_function_table_t g_codec_function_table[STREAM_TYPE_COUNT] = {
#ifdef VORBIS_CODEC
    {
        &VORBISDecoderGetMemorySize,
        &VORBISDecoderInit,
        &VORBISDecoderDecode,
        NULL,
        NULL,
        &VORBISDecoderSeek,
        &VORBISDecoderGetIOFrameSize,
    },
#else
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    }
#endif
};
```

- Enable or Disable Decoder
    -  Define `VORBIS_CODEC` macro in audio_cfg.h file.
    -  Comment this macro if you want to disable VORBIS Decoder otherwise keep it defined in order to enable the decoder.
- Add Extract Metadata API for the decoder
    -  Add extract metadata API source file for the decoder at streamer/cci/metadata/src/vorbis folder.
    -  Add this code in extract metadata lib project space.
    -  Build the extract metadata lib and copy that lib to `libs` folder.
    -  Add the desired stream type into `ccidec_extract_meta_data` API (in codecextractmetadata.c file) to call VORBIS Decoder extract metadata API.
- Add stream type of the new decoder in the stream type enum audio_stream_type_t in codec_interface_public_api.h
    -  Stream type of the decoder in stream type enum and decoder APIs in decoder function table must be in the same sequence.

### Common encoder interface
Please see the following section about the [cei](CeiEncoder.md).

## Maestro performance

### Memory information
The memory usage of the framework (data obtained from IAR compiler) in bytes is:
* Code/flash = 77678
* RAM = 5013

| Section | Size |
|--------:|-----:|
| .text | 77678 |
| .bss | 5013 |

### CPU usage
The performance of the decoder was measured using the real hardware platform (RT1060).
* CPU core clock in MHz: 600.

| Pipeline type | Performance MIPS of codec (in MHz)  |
|:----------:|:----------------------------------:|
|audio source -> audio sink| ~10.26 MHz |
|audio source -> file sink| ~9.84 MHz |
|file source (8-channel PCM) -> audio sink| ~16.5 MHz |

For performance details about the supported codecs please see [AAC decoder](aacdec.md), [FLAC decoder](flacdec.md), [MP3 decoder](mp3dec.md) and [WAV decoder](wavdec.md) documents.

## Opus codec
For opus decoder documentation please see following link: [opus](https://opus-codec.org/docs/opus_api-1.3.1/).

## Opus file
The following library is used for gaining information from the Ogg Opus audio streams: [opusfile](https://opus-codec.org/docs/opusfile_api-0.12/index.html).
