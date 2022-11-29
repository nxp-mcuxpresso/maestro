# Getting started with Maestro framework

## Introduction

This document describes the basic usage of the Maestro Audio Framework. This framework intends to enable the chaining of basic audio processing blocks (called "elements"). These blocks then form stream processing objects ("pipeline").
This pipeline can be used for multiple audio processing use cases.
The processing blocks can include (but are not limited to) different audio sources (for example file or microphone), decoders or encoders, filters or effects, and audio sinks.
For detailed description of the Maestro framework, please refer to the [programmer's guide](doxygen/ProgrammersGuide.md).

Framework overview is depicted in the following picture:

![maestro overview](doxygen/common/maestroApp.svg)

## Supported examples

- maestro_playback
- maestro_playback_8ch_96kHz
- maestro_record
- maestro_usb_mic
- maestro_usb_speaker

The examples can be found in the audio_examples folder of the desired board. The demo applications are based on FreeRTOS and use multiple tasks to form the application functionality.

### maestro_playback

- **file**: perform audio file decode (EAP post processing, if enabled in MCUXpresso IDE) and playback to the line-out (speaker)
    - *start*:  start the playback of a currently selected file
    - *stop*:   stop the current playback
    - *pause*:  toggle pause/play
    - *track filename*: select the audio file to be played
    - *update*: Apply current EAP parameters without attribute value or switch to preset 1-10
    - *set*:    Apply current EAP parameters without attribute value or switch to preset 1-10
    - *get*:    Sync actual EAP parameters from the library to ATT config structures.
    - *volume*: Set the volume of the playback

### maestro_playback_8ch_96kHz

- **file**: read raw pcm file (EAP post processing - just for stereo files, if enabled in MCUXpresso IDE) and playback to the line-out (speaker)
    - *start*:  start the playback of a currently selected file (default setting is 8 channels)
    - *stop*:   stop the current playback
    - *pause*:  toggle pause/play
    - *track filename [channel_number]*: select the audio file to be played, if it is stereo file, specify 2 channels, default is 8 channels
    - *update*: Apply current EAP parameters without attribute value or switch to preset 1-10
    - *set*:    Apply current EAP parameters without attribute value or switch to preset 1-10
    - *get*:    Sync actual EAP parameters from the library to ATT config structures.
    - *volume*: Set the volume of the playback

### maestro_record

- **record_mic**: records MIC audio and either:
    - *audio*:    playback on a codec (line-out/speaker)
    - *file [file_name]*: store samples from onboard microphone to a file on sd card
    - *vit*:      on supported boards perform VoiceSeeker pre-processing, then perform voice recognition (VIT)
- **opus_encode**: perform opus encoding on a sample pcm file and compare with a reference

### maestro_usb_mic

- **usb_mic**: record MIC audio and playback to the USB port as an audio 2.0 microphone device

### maestro_usb_speaker

- **usb_speaker**: play data from the USB port as an audio 2.0 speaker device

## Example applications overview

To set up the audio framework properly, it is necessary to create a streamer with `streamer_create` API. It is also essential to set up the desired hardware peripherals using the functions described in `streamer_pcm.h`.
The Maestro example projects consist of several files regarding the audio framework. The initial file is `main.c` with code to create multiple tasks.
For features including SD card (in the maestro_playback examples, reading a file from SD card is supported and in maestro_record writing to SD card is currently supported) the `APP_SDCARD_Task` is created.
The command prompt and connected functionalities are handled by `APP_Shell_Task`. In the maestro_playback example applications, there is also `APP_main_Task` that handles states of the application using a state machine in
the `eap_att.c` source file. This is also used for application control from the EAP Audio Tuning Tool. User commands are handled by functions in `cmd.c`.

One of the most important parts of the configuration is the `streamer_pcm.c` where the initialization of the hardware peripherals, input and output buffer management can be found. For further information please see also `streamer_pcm.h`

In the Maestro USB examples (maestro_usb_mic and maestro_usb_speaker), the USB configuration is located in the `usb_device_descriptor.c`, `audio_microphone.c` and `audio_speaker.c` files. For further information please see also `usb_device_descriptor.h`, `audio_microphone.h` and `audio_speaker.h`.

In order to be able to get the messages from the audio framework, it is necessary to create a thread for receiving the messages from the streamer, which is usually called a `Message Task`.
The message thread is placed in the `app_streamer.c` file, reads the streamer message queue, and reacts to the following messages:

- STREAM_MSG_ERROR -  stops the streamer and exits the message thread
- STREAM_MSG_EOS - stops the streamer and exits the message thread
- STREAM_MSG_UPDATE_DURATION - prints info about the stream duration
- STREAM_MSG_UPDATE_POSITION - prints info about current stream position
- STREAM_MSG_CLOSE_TASK - exits the message thread

## Commands

### version

Prints component versions

### file

`File` command is supported in the maestro_playback and maestro_playback_8ch_96kHz examples.
In maestro_playback the `file` command creates a streamer pipeline described in function `STREAMER_file_Create` in file `app_streamer.c`. The crucial part in this is defining pipeline type as `STREAM_PIPELINE_FILESYSTEM` or `STREAM_PIPELINE_AUDIO_PROC`.
Depending on whether the `EAP_PROC` is defined this sets up the processing chain from file source to audio sink with decoder and potentially EAP processing in between.
Playback itself can be started with `STREAMER_Start` function.
The pipeline can consist of the following elements:

- TYPE_ELEMENT_FILE_SRC
- TYPE_ELEMENT_DECODER
- TYPE_ELEMENT_AUDIO_PROC (if `EAP_PROC` is defined)
- TYPE_ELEMENT_AUDIO_SINK

Other possible element types could be found in the `streamer_api.h`.

In maestro_playback_8ch_96kHz the `file` command creates a pipeline with PCM file source and audio sink. EAP processing is used for stereo files only with stereo output. If 8 channel PCM file is provided it is output to all 8 channels.
Using this command user starts the creation of the pipeline with the `STREAMER_PCM_Create` function in `app_streamer.c`. Depending on if EAP is used, the pipeline type is either `STREAM_PIPELINE_PCM_AUDIO_PROC_AUDIO` or `STREAM_PIPELINE_PCM_AUDIO`.
The properties of the PCM file are set in the `app_streamer.c` file using file source properties sent to the streamer:
`PROP_FILESRC_SET_SAMPLE_RATE`  - default value is 96000 [Hz]
`PROP_FILESRC_SET_NUM_CHANNELS` - default value is 8
`PROP_FILESRC_SET_BIT_WIDTH`    - default value is 32

Each of the elements has several properties that can be accessed using `streamer_get_property`. These properties allow a user to change the values of the appropriate elements. The list of properties can be found in `streamer_element_properties.h`. See the example of setting property value in the following piece of code from `app_streamer.c`:


```c

ELEMENT_PROPERTY_T prop;
eap_args.preset_num          = *eap_par;
POST_PROCESS_DESC_T eap_proc = {EAP_Initialize_func, EAP_Execute_func, EAP_Deinit_func, &eap_args, 0};

prop.prop = PROP_AUDIOSINK_ADD_POST_PROCESS;
prop.val  = (uintptr_t)&eap_proc;

streamer_set_property(handle->streamer, prop, true);

prop.prop = PROP_FILESRC_SET_CHUNK_SIZE;
prop.val  = 960;

streamer_set_property(handle->streamer, prop, true);

```

Some of the predefined values can be found in the `streamer_api.h`.

### record_mic

`Record_mic` command is supported in the maestro_record example. The command creates pipeline described in function `STREAMER_mic_Create` in file `app_streamer.c`. The pipeline is selected with `STREAM_PIPELINE_PCM`, `STREAM_PIPELINE_MIC2FILE` or `STREAM_PIPELINE_VIT` in `pipeline_type` parameter. Depending on the command option, that may be either `audio`, `file` or `vit`. This configuration takes samples from the microphone input and sends them to the audio sink (speaker(s) or filesystem) with optional processing in between.
The pipeline can consist of the following elements:

- TYPE_ELEMENT_AUDIO_SRC
- TYPE_ELEMENT_AUDIO_SINK or TYPE_ELEMENT_FILE_SINK or TYPE_ELEMENT_VIT_SINK

Other possible element types and some of the predefined values could be found in the `streamer_api.h`.

### usb_mic

`Usb_mic` command is only supported in the maestro_usb_mic example. The command creates pipeline described in function `STREAMER_mic_Create` in file `app_streamer.c`. The pipeline is selected with `STREAM_PIPELINE_PCM` in `pipeline_type` parameter. This configuration takes samples from the microphone input and sends them to the audio sink (USB port).
The pipeline can consist of the following elements:
- TYPE_ELEMENT_AUDIO_SRC
- TYPE_ELEMENT_AUDIO_SINK

Other possible element types and some of the predefined values could be found in `streamer_api.h`.

### usb_speaker

`Usb_speaker` command is only supported in the maestro_usb_speaker example. The command creates pipeline described in function `STREAMER_speaker_Create` in file `app_streamer.c`. The pipeline is selected with `STREAM_PIPELINE_PCM` in `pipeline_type` parameter. This configuration takes samples from the USB port and sends them to the audio sink (speaker output).
The pipeline can consist of the following elements:
- TYPE_ELEMENT_AUDIO_SRC
- TYPE_ELEMENT_AUDIO_SINK

Other possible element types and some of the predefined values could be found in `streamer_api.h`.

## Configuration options

Users can change the pipeline type when creating the streamer object. Currently tested options are:
- `STREAM_PIPELINE_PCM`
- `STREAM_PIPELINE_NETBUF`
- `STREAM_PIPELINE_FILESYSTEM`
- `STREAM_PIPELINE_MIC2FILE`
- `STREAM_PIPELINE_VIT`
- `STREAM_PIPELINE_AUDIO_PROC`
- `STREAM_PIPELINE_MEM`
- `STREAM_PIPELINE_OPUS_MEM2MEM`
- `STREAM_PIPELINE_PCM_AUDIO_PROC_AUDIO`
- `STREAM_PIPELINE_PCM_AUDIO`

In order to create a user defined pipeline, please see the [programmer's guide](doxygen/ProgrammersGuide.md).

## Supported features

The current version of the audio framework supports some optional features. These can be limited to some MCU cores or boards variants.

### Codecs

The maestro_playback can play multiple files with the `file` option (`STREAM_PIPELINE_FILESYSTEM`). The `opus` (as standalone or as a part of `ogg` encapsulation) and `mp3` codecs are supported for now. For detailed code handling this file extension please check `cmd.c` file and `shellFile()` function.
Supported codecs and its options are:

| Decoder               | Sample rates [kHz]                                            | Number of channels                | Bit depth                 |
|-----------------------|---------------------------------------------------------------|-----------------------------------|---------------------------|
| AAC                   | 8, 11.025, 16, 22.050, 24, 32, 44.1, 48                       | 1, 2 (mono/stereo)                | 16                        |
| FLAC                  | 8, 11.025, 16, 22.050, 24, 32, 44.1, 48                       | 1, 2 (mono/stereo)                | 16                        |
| MP3                   | 8, 11.025, 16, 22.050, 32, 44.1, 48                           | 1, 2 (mono/stereo)                | 16                        |
| opus                  | 8, 16, 24, 48                                                 | 1, 2 (mono/stereo)                | 16                        |
| WAV                   | 8, 11.025, 16, 22.050, 32, 44.1, 48                           | 1, 2 (mono/stereo)                | 8, 16, 24                 |

For more details about the codecs please see following documents:
[AAC decoder](doxygen/aacdec.md), [FLAC decoder](doxygen/flacdec.md), [MP3 decoder](doxygen/mp3dec.md), [opus](https://opus-codec.org/docs/opus_api-1.3.1/), [WAV decoder](doxygen/wavdec.md)

Using maestro_playback_8ch_96kHz example users can play PCM files from the SD card and output either 2 or 8 channels with 96kHz sampling rate and 32bit depth (just RT1060-EVKB with AUD-EXP-42448 expansion board supported).

### VIT

VIT can be enabled for the `record_mic` command using the `vit` option. In the `STREAMER_mic_Create` function in the `app_streamer.c` file, the `VIT_PROC` preprocessor define ensures the creation of pipeline with VIT (`STREAM_PIPELINE_VIT`). More details about VIT are available in the VIT package, which is located in `middleware\vit\{platform}\Doc\`(depending on the platform) or via following [link](https://nxp.com/vit).

### EAP

EAP can be enabled with `EAP_PROC` or `EAP32_PROC` preprocessor define for the respective EAP version. With the `file` command using the `set` or `update` option user can switch the different EAP presets as mentioned in the `maestro_playback` section.
In the `STREAMER_file_Create` function in the `app_streamer.c` file, the `EAP_PROC` definition ensures the creation of pipeline with EAP (`STREAMER_PIPELINE_AUDIO_PROC`).
For the platforms that support EAP32 (RT1060-EVKB) users can create the pipeline with `EAP32_PROC` defined. The function that creates the streamer is `STREAMER_PCM_Create` in the `app_streamer.c`. Created pipeline has `STREAM_PIPELINE_PCM_AUDIO_PROC_AUDIO` type.
More details about EAP are available in the EAP package, which is located in `middleware\EAP\Doc\` or via following [link](https://nxp.com/eap).