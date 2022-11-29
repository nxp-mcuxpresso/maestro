# CEI encoder

The Maestro streamer contains an element adapting an extensible set of audio encoders in the form of functions conforming to the CEI (Common Encoder Interface). This element enables the user to choose and configure a suitable encoder at runtime.

## Header files

CEI itself and the CEI encoders are using following header files, in which you may be interested:

- `cei.h` - contains types used by the element itself and an encoder implementing the CEI
- `cei_enctypes.h` - contains a list of possible encoders and types used for interfacing with a CEI encoder
- `cei_table.h` - contains a table of functions implementing integrated CEI encoders

## Instantiating the element

This element's index is `ELEMENT_ENCODER_INDEX` and its type is `TYPE_ELEMENT_ENCODER`, as defined in `streamer_api.h`. It has one source pad (data input) and one sink pad (data output). It is initialized like any other element, meaning that it is instantiated and inserted into the pipeline using the `create_element`, `add_element_pipeline` and `link_elements` functions. Inversely, for destroying the element, the `unlink_elements`, `remove_element_pipeline` and `destroy_element` are used. This element alone does not depend on any additional software layers other than these required by the Maestro streamer itself, so no pre-initialization before this element instantiation is necessary.

## Element properties

Use Maestro streamer property API (`streamer_set_property` and `streamer_get_property`) for setting or getting these. The constants are defined in `streamer_element_properties.h`.

- `PROP_ENCODER_CHUNK_SIZE`
    - **Synopsis:** Determines the length of a chunk pulled from the sibling of the source pad and essentially influences the size of allocated buffers. If the actual amount of data pulled is smaller, the rest is zero-filled.
    - **Type:** unsigned 32-bit integer
    - **Default value:** `1920`
    - **Constraints:**
        - Must be bigger than zero, otherwise `STREAM_ERR_INVALID_ARGS` is returned.
        - Cannot be changed if the actual encoder has been created. If done so, `STREAM_ERR_ELEMENT_BAD_STATUS` is returned.

- `PROP_ENCODER_TYPE`
    - **Synopsis:** Determines the exact encoder (CEI implementation) to be used.
    - **Type:** `CeiEncoderType` (`cei_enctypes.h`)
    - **Default value:** `CEIENC_LAST`
    - **Constraints:**
        - Must not be equal to `CEIENC_LAST`, otherwise `STREAM_ERR_INVALID_ARGS` will be returned.
        - Selected encoder must be implemented, otherwise `STREAM_ERR_INVALID_ARGS` will be returned.
        - Cannot be changed if the actual encoder has been created. If done so, `STREAM_ERR_ELEMENT_BAD_STATUS` will be returned.
    - **Behaviour influenced:** The encoder element process function will return `FLOW_ERROR` if this property isn't set.

- `PROP_ENCODER_CONFIG`
    - **Synopsis:** Determines encoder-specific configuration (application, bitrate, ...).
    - **Type:** Pointer to the encoder-specific configuration structure.
    - **Default value:** Determined by the encoder.
    - **Constraints**:
        - The encoder has to be configurable. If it is not, `STREAM_ERR_ERR_GENERAL` will be returned on any access.
        - The structure has to conform to the encoder requirements. If the encoder returns an error code, `STREAM_ERR_GENERAL` will be returned.

- `PROP_ENCODER_BITSTREAMINFO`
    - **Synopsis:** Specifies information about the incoming bitstream (sample rate, sample depth, ...).
    - **Type:** Pointer to `CeiBitstreamInfo` (`cei_enctypes.h`).
    - **Default value:**
    ```c
        (CeiBitstreamInfo) {
            .sample_rate = 0,
            .num_channels = 0,
            .endian = AF_LITTLE_ENDIAN,
            .sign = TRUE,
            .sample_size = 0,
            .interleaved = TRUE
        }
    ```
    - **Constraints:**
        - Cannot be changed if the actual encoder has been created. If done so, `STREAM_ERR_ELEMENT_BAD_STATUS` will be returned.
        - As of now, only bitstreams containing 16-bit interleaved (if 2 or more channels will be encoded) samples are supported. If anything else was set to the `sample_size`  and `interleaved` members, `STREAM_ERR_INVALID_ARGS` will be returned.
    - **Behaviour influenced:**
        - Given the characteristics of some elements available, different packets of data (header and payload, referred to as "chunk" above) may be pulled by this element. Each packet can contain a different header, which may or may not contain useful information about the bitstream. If a packet with the `AudioPacketHeader` (`todofile.h`) is pulled at first and any other iteration of the streamer pipeline, the bitstream parameters configured by this property are implicitly available and are not expected to be specified by the user. Other packet header types (such as `RawPacketHeader`) don't contain any bitstream parameters and require the user to specify the parameters manually using this property. Failure to do so will result in the element's process function returning `FLOW_ERROR`. Same situation will occur if a packet with the `AudioPacketHeader` is received and its contents differ from the already acquired bitstream parameters.
        - As of now, CEI is defined to work with 16-bit signed little-endian (s16le) samples, which are interleaved if the bitstream contains more than one channels. This element handles endianness and unsigned to signed conversion.

## CEI definition - implementing your own encoder

The CEI defines following function pointer types:

- `CeiFnGetMemorySize`: Returns number of bytes required for encoder state for a given number of channels.
- `CeiFnEncoderInit`: Initialize an encoder for a given sample rate and channel count.
- `CeiFnEncoderGetConfig`: Copy current or default configuration to a given structure pointer.
- `CeiFnEncoderSetConfig`: Configure the encoder from a given structure pointer.
- `CeiFnEncode`: Encode a given buffer to a given output buffer.

Detailed descriptions of function behaviour, parameters and expected return values are available as docblocks in the `cei.h` file.

Each encoder is implemented as a set of pointers pointing to functions conforming to these types, grouped in the `CeiEncoderFunctions` structure. Specifying the `CeiEncoderGetConfig fnGetConfig` and `CeiFnEncoderSetConfig fnSetConfig` members is optional, as an encoder does not have to be configurable. If so desired, specify `NULL`. Implementation of the remaining functions is mandatory, however. If at least one of these functions isn't implemented and `NULL` is specified instead, the encoder will be considered as not implemented.

To register an implemented encoder with the element, add a new entry to the `CeiEncoderType` enum and add the `CeiEncoderFunctions` struct value to the table `CeiEncoderFunctions ceiEncTable[]` located in the `cei_table.h` header file. Note and match the order of items in that table, as a `CeiEncoderType` value is used as an index. Same goes for the `size_t ceiEncConfigSizeTable[]`. If configuration is not applicable, specify `0` at the appropriate index. If configuration is applicable, describe the configuration structure in the `cei_enctypes.h` header file and add its size to that table.

