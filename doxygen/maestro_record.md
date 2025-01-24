# Maestro record example

## Table of content
- [Overview](#overview)
- [Hardware requirements](#hardware-requirements)
- [Hardware modifications](#hardware-modifications)
- [Preparation](#preparation)
- [Running the demo](#running-the-demo)
- [Example configuration](#example-configuration)
- [Functionality](#functionality)
- [States](#states)
- [Commands in detail](#commands-in-detail)
- [Processing Time](#processing-time)

## Overview
The Maestro record example demonstrates audio processing on the ARM cortex core utilizing the Maestro Audio Framework library.

The application is controlled by commands from a shell interface using serial console.

Depending on target platform or development board there are different modes and features of the demo supported.

- **Loopback** - The application demonstrates a loopback from the microphone to the speaker without any audio processing. Mono, stereo or multichannel mode can be used, depending on the hardware, see [table](#overview) below.
- **File recording** - The application takes audio samples from the microphone inputs and stores them to an SD card as an PCM file. The PCM file has following parameters:
    - Mono and stereo : 2 channels, 16kHz, 16bit width
    - Multi-channel (AUD-EXP-42448): 6 channels, 16kHz, 32bit width
- **Voice control** - The application takes audio samples from the microphone input and uses the VIT library to recognize wake words and voice commands. If a wake word or a voice command is recognized, the application write it to the serial terminal.
- **Encoding** - The application takes PCM samples from memory and sends them to the Opus encoder. The encoded data is stored in memory and compared to a reference. The result of the comparison is finally written into the serial terminal.

As shown in the table below, the application is supported on several development boards, and each development board may have certain limitations, some development boards may also require hardware modifications or allow to use of an audio expansion board. Therefore, please check the supported features and [Hardware modifications](#hardware-modifications) or [Example configuration](#example-configuration) sections before running the demo.

<div class="wy-table-responsive">
    <table class="audio_FW_spec_table">
        <thead>
            <tr >
                <th colspan="1">Mode</th>
                <th colspan="3">Loopback</th>
                <th colspan="3">File recording</th>
                <th colspan="5">Voice control</th>
                <th colspan="1">Encoding</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td colspan="1" rowspan="2" >Feature</td>
                <td colspan="3">Audio input / output [num of channels]</td>
                <td colspan="3">Audio inputs [num of channels]</td>
                <td colspan="3">Audio inputs [num of channels]</td>
                <td colspan="2">Additional libraries</td>
                <td colspan="1">Encoder</td>
            </tr>
            <tr>
                <td colspan="1">On board codec</td>
                <td colspan="1">DMICs</td>
                <td colspan="1">aud-exp-42448</td>
                <td colspan="1">On board codec</td>
                <td colspan="1">DMICs</td>
                <td colspan="1">aud-exp-42448</td>
                <td colspan="1">On board codec</td>
                <td colspan="1">DMICs</td>
                <td colspan="1">aud-exp-42448</td>
                <td colspan="1">VIT</td>
                <td colspan="1">VoiceSeeker</td>
                <td colspan="1">OPUS</td>
            </tr>
            <tr>
                <td colspan="1">EVKC-MIMXRT1060</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1 / 1</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_modification">1-6 / 1-6</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1</td>
                <td colspan="1" >X</td>
                <td colspan="1" class="audio_FW_spec_table_modification">1-6</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1</td>
                <td colspan="1" >X</td>
                <td colspan="1" class="audio_FW_spec_table_modification">1-2</td>
                <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
                <td colspan="1" class="audio_FW_spec_table_modification">OK</td>
                <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            </tr>
            <tr>
                <td colspan="1">EVKB-MIMXRT1170</td>
                <td colspan="1" class="audio_FW_spec_table_supported">0 / 1-2</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
                <td colspan="1" >X</td>
                <td colspan="1" >X</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
                <td colspan="1" >X</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
                <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
                <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            </tr>
            <tr>
                <td colspan="1">LPCXpresso55s69</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1-2 / 1-2</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1-2 / 1-2</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_modification">OK</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
            </tr>
            <tr>
                <td colspan="1">EVK-MCXN5XX</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1-2 / 1-2</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            </tr>
            <tr>
                <td colspan="1">RW612BGA</td>
                <td colspan="1" class="audio_FW_spec_table_supported">0 / 1-2</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
                <td colspan="1">X</td>
                <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
                <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
                <td colspan="1">X</td>
            </tr>
        </tbody>
    </table>
</div>
<div style="font-size:.8em"> - <span class="audio_FW_spec_table_supported">Dark green</span> - Fully supported and enabled by default.</div>
<div style="font-size:.8em"> - <span class="audio_FW_spec_table_limited">Orange</span> - Supported with some limitations and enabled by default. See the limitations section below.</div>
<div style="font-size:.8em"> - <span class="audio_FW_spec_table_modification">Light green</span> - Supported, but only available after some SW or HW modification. More information about modification can be found in the [Example configuration](#example-configuration) section.</div>
<div style="font-size:.8em"> - <span>X</span> - Not supported.</div>

**Limitations:**
- Note:
    - *LPCXPresso55s69* - MCUXpresso IDE project default debug console is semihost
- Addition labraries
    - **VIT:**
        - The VIT is supported only in the MCUXpresso IDE and ARMGCC.
        - *LPCXpresso55s69* - The VIT is disabled by default due to insufficient memory. To enable it, see the [Example configuration](#example-configuration) section.
        - *EVK-MCXN5XX* - Some VIT models can't fit into memory. In order to free some space it is necessary to disable SD card handling. To disable it, see the [Example configuration](#example-configuration) section.
    - **VoiceSeeker:**
        - The VoiceSeeker is supported only in the MCUXpresso IDE and ARMGCC.
- Encoder
    - **OPUS:**
        - *LPCXpresso55s69* - The encoder is not supported due to insufficient memory.
- The File recording mode is not supported on *RW612BGA* development board due to missing SD card slot.

**Known issues:**
- *EVKB-MIMXRT1170* - After several tens of runs (the number of runs is not deterministic), the development board restarts because a power-up sequence is detected on the RESET pin (due to a voltage drop).

More information about supported features can be found on the [Supported features](supported_features.md) page.

## Hardware requirements
- Desired development board
- Micro USB cable
- Headphones with 3.5 mm stereo jack
- Personal computer
- Optional:
    - SD card for file output
    - Audio expansion board [AUD-EXP-42448 (REV B)](https://www.nxp.com/part/AUD-EXP-42448#/)
- *LPCXpresso55s69:*
    - Source of sound with 3.5 mm stereo jack connector

## Hardware modifications
Some development boards need some hardware modifications to run the application. If the development board is not listed here, its default setting is required.
- *EVKB-MIMXRT1170:*
    1. Please remove below resistors if on board wifi chip is not DNP:
        - R228, R229, R232, R234
    2. Please make sure R136 is weld for GPIO card detect.
- *EVK-MCXN5XX:*
    - Short: JP7 2-3, JP8 2-3, JP10 2-3, JP11 2-3
- *RW612BGA:*
    - Connect: JP50; Disconnect JP9, JP11

## Preparation
1. Connect a micro USB cable between the PC host and the debug USB port on the development board
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Insert the headphones into the Line-Out connector (headphone jack) on the development board.
5. *LPCXpresso55s69:*
    - Insert source of sound to audio Line-In connector (headphone jack) on the development board.
6. Either press the reset button on your development board or launch the debugger in your IDE to begin running the demo.

## Running the demo
When the example runs successfully, you should see similar output on the serial terminal as below:
```
    *******************************
    Maestro audio record demo start
    *******************************

    Copyright  2022  NXP
    [APP_SDCARD_Task] start
    [APP_Shell_Task] start

    >> [APP_SDCARD_Task] SD card drive mounted
```

Type `help` to see the command list. Similar description will be displayed on serial console:

```
    >> help

    "help": List all the registered commands

    "exit": Exit program

    "version": Display component versions

    "record_mic": Record MIC audio and perform one (or more) of following actions:
    - playback on codec
    - perform VoiceSeeker processing
    - perform voice recognition (VIT)
    - store samples to a file.

    USAGE: record_mic [audio|file|<file_name>|vit] 20 [<language>]
    The number defines length of recording in seconds.

    Please see the project defined symbols for the languages supported.
    Then specify one of: en/cn/de/es/fr/it/ja/ko/pt/tr as the language parameter.
    For voice recognition say supported WakeWord and in 3s frame supported command.
    Please note that this VIT demo is near-field and uses 1 on-board microphone.

    NOTES: This command returns to shell after the recording is finished.
           To store samples to a file, the "file" option can be used to create a file
           with a predefined name, or any file name (without whitespaces) can be specified
           instead of the "file" option.

    "opus_encode": Initializes the streamer with the Opus memory-to-memory pipeline and
    encodes a hardcoded buffer.
```
Details of commands can be found [here](#commands-in-detail).

## Example configuration
The example can be configured by user. Before configuration, please check the [table](#overview) to see if the feature is supported on the development board.
- **Connect AUD-EXP-42448:**
    - *EVKC-MIMXRT1060:*
        1. Disconnect the power supply for safety reasons.
        2. Insert AUD-EXP-42448 into J19 to be able to use the CS42448 codec for multichannel output.
        3. Uninstall J99.
        4. Set the `DEMO_CODEC_WM8962` macro to `0` in the `app_definitions.h` file
        5. Set the `DEMO_CODEC_CS42448` macro to `1` in the `app_definitions.h` file.
        6. Enable VoiceSeeker, see point bellow.
    - *Note:*
        - The audio stream is as follows:
            - Stereo INPUT 1 (J12) -> LINE 1&2 OUTPUT (J6)
            - Stereo INPUT 2 (J15) -> LINE 3&4 OUTPUT (J7)
            - MIC1 & MIC2 (P1, P2) -> LINE 5&6 OUTPUT (J8)
            - Insert the headphones into the different line outputs to hear the inputs.
            - To use the Stereo INPUT 1, 2, connect an audio source LINE IN jack.
- **Enable VoiceSeeker:**
    - On some development boards the VoiceSeeker is enabled by default, see the [table](#overview) above.
    - If more than one channel is used and VIT is enabled, the VoiceSeeker that combines multiple channels into one must be used, as VIT can only work with mono signal.
    - It is necessary to add `VOICE_SEEKER_PROC` symbol to preprocessor defines on project level:
        - (Project -> Properties -> C/C++ Build -> Settings -> MCU C Compiler -> Preprocessor)
- **Enable VIT:**
    - *LPCXpresso55s69:*
        1. Remove `SD_ENABLED` and `STREAMER_ENABLE_FILE_SINK` symbols from preprocessor defines on project level.
        2. Add `VIT_PROC` symbol to preprocessor defines on project level:
            - (Project -> Properties -> C/C++ Build -> Settings -> MCU C Compiler -> Preprocessor)
        3. Change the `DEMO_MIC_CHANNEL_NUM` symbol value from `2` to `1` in the `app_definitions.h` file
- **VIT model generation:**
    - For custom VIT model generation (defining own wake words and voice commands) please use https://vit.nxp.com/
- **Disable SD card handling:**
    - To disable it, remove `SD_ENABLED` and `STREAMER_ENABLE_FILE_SINK` symbols from preprocessor defines on project level:
        - (Project -> Properties -> C/C++ Build -> Settings -> MCU C Compiler -> Preprocessor)

## Functionality

The `record_mic` or `opus_encode` command calls the `STREAMER_mic_Create` or `STREAMER_opusmem2mem_Create` function from the `app_streamer.c` file depending on the selected mode.

- When the *Loopback* mode is selected, the command calls the `STREAMER_mic_Create` function that creates a pipeline with the following elements:
    - ELEMENT_MICROPHONE_INDEX
    - ELEMENT_SPEAKER_INDEX
- When the *File recording* mode is selected, the command calls the `STREAMER_mic_Create` function that creates a pipeline with the following elements:
        - ELEMENT_MICROPHONE_INDEX
        - ELEMENT_FILE_SINK_INDEX
- When the *Voice control* mode is selected, the command calls the `STREAMER_mic_Create` function that creates a pipeline with the following elements:
        - ELEMENT_MICROPHONE_INDEX
        - ELEMENT_VOICESEEKER_INDEX (If `VOICE_SEEKER_PROC` is defined)
        - ELEMENT_VIT_INDEX
- When the Encoding mode is selected, the command calls the `STREAMER_opusmem2mem_Create` function that creates a pipeline with the following elements:
        - ELEMENT_MEM_SRC_INDEX
        - ELEMENT_ENCODER_INDEX
        - ELEMENT_MEM_SINK_INDEX

Recording itself can be started with the `STREAMER_Start` function.

Each of the elements has several properties that can be accessed using the `streamer_get_property` or `streamer_set_property` function. These properties allow a user to change the values of the appropriate elements. The list of properties can be found in `streamer_element_properties.h`. See the example of setting property value in the following piece of code from the `app_streamer.c` file:

```c
    ELEMENT_PROPERTY_T prop;

    prop.prop = PROP_MICROPHONE_SET_NUM_CHANNELS;
    prop.val  = DEMO_MIC_CHANNEL_NUM;
    streamer_set_property(handle->streamer, 0, prop, true);

    prop.prop = PROP_MICROPHONE_SET_BITS_PER_SAMPLE;
    prop.val  = DEMO_AUDIO_BIT_WIDTH;
    streamer_set_property(handle->streamer, 0, prop, true);

    prop.prop = PROP_MICROPHONE_SET_FRAME_MS;
    prop.val  = DEMO_MIC_FRAME_SIZE;
    streamer_set_property(handle->streamer, 0, prop, true);

    prop.prop = PROP_MICROPHONE_SET_SAMPLE_RATE;
    prop.val  = DEMO_AUDIO_SAMPLE_RATE;
    streamer_set_property(handle->streamer, 0, prop, true);
```
Some of the predefined values can be found in the `streamer_api.h`.

## States
The application can be in 2 different states:
- Idle
- Running

## Commands in detail
- [help, version](#help-version)
- [record_mic audio \<time\>](#record_mic-audio-time)
- [record_mic file \<time\>](#record_mic-file-timerecord_mic-file_name-time)
- [record_mic \<file_name\> \<time\>](#record_mic-file-timerecord_mic-file_name-time)
- [record_mic vit \<time\> \<language\>](#record_mic-vit-time-language)
- [opus_encode](#opus_encode)

Legend for diagrams:
```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    A((State)):::state
    B{Condition}:::condition
    C[Error message]:::error
    D[Process function]:::function
```

### help, version

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    A((Idle)):::state --> C[Write help or version]:::function
    B((Running)):::state --> C
    C --> E((No state
    change)):::state
```

### record_mic audio \<time\>

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    B((Idle)):::state --> D{time
    > 0 ?}:::condition
    D -- Yes --> F[recording]:::function
    D -- No --> E[Error: Record length
    must be greater than 0]:::error
    E --> B
    F --> C((Running)):::state
    C -->G{time
    expired?}:::condition
    G -- No --> C
    G -- Yes --> B

```

### record_mic file \<time\>\/record_mic \<file_name\> \<time\>

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    B((Idle)):::state --> C{time
    > 0 ?}:::condition
    C -- Yes --> D{SD card
    inserted?}:::condition
    C -- No --> E[Error: Record length
    must be greater than 0]:::error
    E --> B
    D -- Yes --> G{Custom
    file name?}:::condition
    G -- Yes --> H[Create custom
    file name]:::function
    G -- No --> I[Create default
    file name]:::function
    H --> J[Recording]:::function
    I --> J
    J --> K((Running)):::state
    K -->L{time
    expired?}:::condition
    L -- No --> K
    L -- Yes --> B
    D -- No --> F[Error: Insert SD
    card first]:::error
    F --> B

```

### record_mic vit \<time\> \<language\>

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    B((Idle)):::state --> C{time
    > 0 ?}:::condition
    C -- Yes --> E{Selected
    language?}:::condition
    C -- No --> D[Error: Record length
    must be greater than 0]:::error
    D --> B
    E -- Yes --> G{Supported
    language?}:::condition
    E -- No --> F[Error: Language
    not selected]:::error
    F -->B
    G -- Yes -->I[Recording with
    voice recognition]:::function
    G -- No -->H[Error: Language not supported]:::error
    H --> B
    I --> J((Running)):::state
    J -->K{time
    expired?}:::condition
    K -- No --> J
    K -- Yes --> B

```

### opus_encode

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    B((Idle)):::state -->C[Encode file]:::function
    C -->D[Check result]:::function
    D -->B
```

## Processing Time

Typical execution times of the streamer pipeline for the EVKC-MIMXRT1060 development board are detailed in the following table. The duration spent on output buffers and reading from the microphone is excluded from traversal measurements. Three measured pipelines were considered. The first involves a loopback from microphone to speaker, supporting both mono and stereo configurations. The second pipeline is a mono voice control setup, comprising microphone and VIT blocks. The final pipeline is a stereo voice control setup, integrating microphone, voice seeker, and VIT blocks.

For further details of execution times on individual elements, please refer to the [Processing Time](processing_time.md) document.

|                                   | streamer  |
|-----------------------------------|-----------|
| microphone -> speaker 1 channel   | 40 &mu;s  |
| microphone -> speaker 2 channels  | 115 &mu;s |
| microphone -> VIT                 | 7.4 ms    |
| microphone -> voice seeker -> VIT | 9.9 ms    |
