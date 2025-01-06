# Maestro playback example

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
The Maestro playback example demonstrates audio processing on the ARM cortex core utilizing the Maestro Audio Framework library.

The application is controlled by commands from a shell interface using serial console and the audio files are read from the SD card.

Depending on target platform or development board there are different modes and features of the demo supported.

- **Standard** - The mode demonstrates playback of encoded files from an SD card with up to 2 channels, up to 48 kHz sample rate and up to 16 bit width. This mode is enabled by default.
- **Multi-channel** - The mode demonstrates playback of raw PCM files from an SD card with 2 or 8 channels, 96kHz sample rate and 32 bit width. The decoders and synchronous sample rate converter are not supported in this mode. The Multi-channel mode is only supported on selected platforms, see the table below. The [Example configuration](#example-configuration) section contains information on how to enable it.

As shown in the table below, the application is supported on several development boards and each development board may have certain limitations, some development boards may also require hardware modifications or allow to use of an audio expansion board. Therefore, please check the supported features and [Hardware modifications](#hardware-modifications) or [Example configuration](#example-configuration) sections before running the demo.

<div class="wy-table-responsive">
<table  class="audio_FW_spec_table">
    <thead>
        <tr >
            <th colspan="1">Mode</th>
            <th colspan="8">Standard</th>
            <th colspan="1">Multi-channel</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan="1" rowspan="2">Feature</td>
            <td colspan="5">Decoder</td>
            <td colspan="1">Sample rate converter</td>
            <td colspan="2">Audio output [num of channels]</td>
            <td colspan="1">Audio output [num of channels]</td>
        </tr>
        <tr>
            <td colspan="1">AAC</td>
            <td colspan="1">FLAC</td>
            <td colspan="1">MP3</td>
            <td colspan="1">OPUS</td>
            <td colspan="1">WAV</td>
            <td colspan="1">SSRC</td>
            <td colspan="1">On board codec</td>
            <td colspan="1">AUD-EXP-42448</td>
            <td colspan="1">AUD-EXP-42448</td>
        </tr>
        <tr>
            <td colspan="1">EVKC-MIMXRT1060</td>
            <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1" class="audio_FW_spec_table_modification">1-2</td>
            <td colspan="1" class="audio_FW_spec_table_modification">2 or 8</td>
        </tr>
        <tr>
            <td colspan="1">EVKB-MIMXRT1170</td>
            <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
        </tr>
        <tr>
            <td colspan="1">LPCXpresso55s69</td>
            <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
            <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1">X</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
        </tr>
        <tr>
            <td colspan="1">EVK-MCXN5X</td>
            <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">OK</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
        </tr>
        <tr>
            <td colspan="1">RW612BGA</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
        </tr>
        <tr>
            <td colspan="1">RW612QFN</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
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
- Decoder:
    - **AAC:**
        - The reference decoder is supported only in the MCUXpresso IDE and ARMGCC.
    - **FLAC:**
        - *LPCXpresso55s69* - When playing FLAC audio files with too small frame size (block size), the audio output may be distorted because the board is not fast enough.
    - **OPUS:**
        - *LPCXpresso55s69* - The decoder is disabled due to insufficient memory may be distorted because the board is not fast enough.
- Sample rate converter:
    - **SSRC:**
        - *LPCXpresso55s69* -  When a memory allocation ERROR occurs, it is necessary to disable the SSRC element due to insufficient memory.

**Known issues:**
- Decoder:
    - **MP3:**
        - The reference decoder has issues with some of the files. One of the channels can be sometimes distorted or missing parts of the signal.

More information about supported features can be found on the [Supported features](supported_features.md) page.

## Hardware requirements
- Desired development board
- Micro USB cable
- Headphones with 3.5 mm stereo jack
- SD card with supported audio files
- Personal computer
- Optional:
    - Audio expansion board [AUD-EXP-42448 (REV B)](https://www.nxp.com/part/AUD-EXP-42448#/)

## Hardware modifications
Some development boards need some hardware modifications to run the application. If the development board is not listed here, its default setting is required.
- *EVKB-MIMXRT1170:*
    1.  Please remove below resistors if on board wifi chip is not DNP:
        - R228, R229, R232, R234
    2.  Please make sure R136 is weld for GPIO card detect.

## Preparation
1. Connect a micro USB cable between the PC host and the debug USB port on the development board.
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Insert the headphones into the Line-Out connector (headphone jack) on the development board.
5. Either press the reset button on your development board or launch the debugger in your IDE to begin running the demo.

## Running the demo
When the example runs successfully, you should see similar output on the serial terminal as below:

```
    *********************************
    Maestro audio playback demo start
    *********************************

    [APP_Main_Task] started

    Copyright  2022  NXP
    [APP_SDCARD_Task] start
    [APP_Shell_Task] start

    >> [APP_SDCARD_Task] SD card drive mounted
```

Type `help` to see the command list. Similar description will be displayed on serial console (*If multi-channel playback mode is enabled, the description is slightly different*):

```
    >> help

    "help": List all the registered commands

    "exit": Exit program

    "version": Display component versions

    "file": Perform audio file decode and playback

      USAGE: file [stop|pause|volume|seek|play|list|info]
        stop              Stops actual playback.
        pause             Pause actual track or resume if already paused.
        volume <volume>   Set volume. The volume can be set from 0 to 100.
        seek <seek_time>  Seek currently paused track. Seek time is absolute time in milliseconds.
        play <filename>   Select audio track to play.
        list              List audio files available on mounted SD card.
        info              Prints playback info.
```
Details of commands can be found [here](#commands-in-detail).

## Example configuration
The example can be configured by user. Before configuration, please check the [table](#overview) to see if the feature is supported on the development board.
- **Enable Multi-channel mode:**
    - Add the `MULTICHANNEL_EXAMPLE` symbol to preprocessor defines on project level.
    - Connect AUD-EXP-42448 (see the point below).
- **Connect AUD-EXP-42448:**
    - *EVKC-MIMXRT1060:*
        1. Disconnect the power supply for safety reasons.
        2. Insert AUD-EXP-42448 into J19 to be able to use the CS42448 codec for multichannel output.
        3. Uninstall J99.
        4. Set the `DEMO_CODEC_WM8962` macro to `0` in the `app_definitions.h` file
        5. Set the `DEMO_CODEC_CS42448` macro to `1` in the `app_definitions.h` file.

## Functionality

The `file play <filename>` command calls the `STREAMER_file_Create` or `STREAMER_PCM_Create` function from the `app_streamer.c` file depending on the selected mode.

- When the *Standard* mode is enabled, the command calls the `STREAMER_file_Create` function that creates a pipeline with the following elements:
    - ELEMENT_FILE_SRC_INDEX
    - ELEMENT_DECODER_INDEX
    - ELEMENT_SRC_INDEX (If `SSRC_PROC` is defined)
    - ELEMENT_SPEAKER_INDEX
- When the *Multi-channel* mode is enabled, the command calls `STREAMER_PCM_Create` function, which creates a pipeline with the following elements:
    - ELEMENT_FILE_SRC_INDEX (PCM format only)
    - ELEMENT_SPEAKER_INDEX
    - *Note:*
        - If the input file is an 8 channel PCM file, output to all 8 channels is available. The properties of the PCM file are set in the `app_streamer.c` file using file source properties sent to the streamer:
            - `PROP_FILESRC_SET_SAMPLE_RATE`  - default value is 96000 [Hz]
            - `PROP_FILESRC_SET_NUM_CHANNELS` - default value is 8
            - `PROP_FILESRC_SET_BIT_WIDTH`    - default value is 32

Playback itself can be started with the `STREAMER_Start` function.

Each of the elements has several properties that can be accessed using the `streamer_get_property` or `streamer_set_property` function. These properties allow a user to change the values of the appropriate elements. The list of properties can be found in `streamer_element_properties.h`. See the example of setting property value in the following piece of code from the `app_streamer.c` file:

```c

    ELEMENT_PROPERTY_T prop;

    EXT_PROCESS_DESC_T ssrc_proc = {SSRC_Proc_Init, SSRC_Proc_Execute, SSRC_Proc_Deinit, &get_app_data()->proc_args};

    prop.prop = PROP_SRC_PROC_FUNCPTR;
    prop.val  = (uintptr_t)&ssrc_proc;

    if (streamer_set_property(streamer, 0, prop, true) != 0)
    {
        return -1;
    }

    prop.prop = PROP_AUDIOSINK_SET_VOLUME;
    prop.val  = volume;
    streamer_set_property(streamer, 0, prop, true);

```

Some of the predefined values can be found in the `streamer_api.h`.

## States
The application can be in 3 different states:
- Idle
- Running
- Paused

In each state, each command can have a different behavior. For more information, see [Commands in detail](#commands-in-detail) section.

## Commands in detail
The applicatin is controlled by commands from the shell interface and the available commands for the selected mode can be displayed using the `help` command. Commands are processed in the `cmd.c` file.

- [help, version](#help-version)
- [file stop](#file-stop)
- [file pause](#file-pause)
- [file volume \<volume\>](#file-volume-volume)
- [file seek \<seek_time\>](#file-seek-seek_time)
- [file play \<filename\>](#file-play-filename)
- [file list](#file-list)
- [file info](#file-info)

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

    A((Idle)):::state --> D[Write help or version]:::function
    B((Running)):::state --> D
    C((Paused)):::state --> D
    D-->E((No state
    change)):::state
```
### file stop

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    B((Idle)):::state --> B
    C((Running)):::state -->E((Idle)):::state
    D((Paused)):::state -->E
```

### file pause

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    B((Idle)):::state --> B
    C((Running)):::state -->E((Paused)):::state
    D((Paused)):::state -->F((Running)):::state
```

### file volume \<volume\>

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    B((Idle)):::state --> M[Error: Play a track first]:::error
    C((Running)):::state --> G{Volume
    parameter
    empty?}:::condition
    D((Paused)):::state --> G
    G -- Yes -->H[Error: Enter volume parameter]:::error
    G -- No -->I{Volume
    in range?}:::condition
    I -- No -->J[Error: invalid value]:::error
    I -- Yes -->K[Set volume]:::function
    J --> L((No state
    change)):::state
    K --> L
    H--> L
```

### file seek \<seek_time\>

The seek argument is only supported in the Standard mode.

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    B((Idle)):::state --> E[Error: First select
    an audio track to play]:::error
    E-->B
    C((Running)):::state --> F[Error: First
    pause the track]:::error
    F --> C
    D((Paused)):::state --> G{Seek
    parameter
    empty?}:::condition
    G --No --> H{AAC file?}:::condition
    G --Yes --> I[Error: Enter
    a seek time value]:::error
    I-->N((Paused)):::state;
    H --Yes -->J[Error: The AAC decoder
    does not support
    the seek command]:::error
    J-->N
    H --No -->K{Seek
    parameter
    positive?}:::condition
    K --No -->L[Error: The seek
    time must be
    a positive value]:::error
    L-->N
    K --Yes -->M[Seek the file]:::function
    M-->N
```

### file play \<filename\>

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    C((Running)):::state --> Z[Error: First stop
    current track]:::error
    D((Paused)):::state --> Z
    B((Idle)):::state --> E{SD Card
    inserted?}:::condition
    E -- No -->F[Error: Insert SD
    card]:::error
    E -- Yes -->G{File
    name
    empty?}:::condition
    G -- Yes -->H[Error: Enter
    file name]:::error
    G -- No -->I{File exists?}:::condition
    I -- No -->O[Error: File
    doesn't exist]:::error
    I -- Yes -->J{Supported
    format?}:::condition
    J -- Yes -->K[Play the track]:::function
    J -- No -->L[Error: Unsupported
    file]:::error
    K -->M((Running)):::state
    L --> W((No state
    change)):::state
    O --> W
    H --> W
    F --> W
    Z --> W
```

### file list

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    B((Idle)):::state --> G{SD Card
    inserted?}:::condition
    C((Running)):::state --> G
    D((Paused)):::state --> G
    G -- Yes -->H[List supported files]:::function
    G -- No -->I[Error: Insert SD card]:::error
    I --> J((No state
    change)):::state
    H --> J
```

### file info

```{mermaid}
flowchart TD
    classDef function fill:#69CA00
    classDef condition fill:#0EAFE0
    classDef state fill:#F9B500
    classDef error fill:#F54D4D

    B((Idle)):::state -->E[Write file info]:::function
    C((Running)):::state -->E
    D((Paused)):::state -->E
    E --> F((No state
        change)):::state
```

## Processing Time

Typical streamer pipeline execution times and their individual elements for the EVKC-MIMXRT1060 development board are presented in the following tables. The time spent on output buffers is not included in the traversal measurements. However, file reading time is accounted for. In the case of the WAV codec, the audio file was accessed in every pipeline run. Therefore, during each run, the file was read from the SD card. However, for the MP3 codec, where data must be processed in complete MP3 frames, the file was not read in every run. Instead, it was read periodically only when the codec buffer did not contain a complete frame of data.

For further details, please refer to the [Processing Time](processing_time.md) document.


|  WAV           | streamer | file_src  | codec     | SSRC_proc | speaker  |
|----------------|----------|-----------|-----------|-----------|----------|
| 48kHz          | 1.1 ms   | 850 &mu;s | 150 &mu;s | 70 &mu;s  | 40 &mu;s |
| 44kHz          | 1.75 ms  | 850 &mu;s | 180 &mu;s | 670 &mu;s | 40 &mu;s |


| MP3                      | streamer | file_src  | codec     | SSRC_proc | speaker  |
|--------------------------|----------|-----------|-----------|-----------|----------|
| 48 kHz with file read    | 2.9 ms   | 2.3 &mu;s | 450 &mu;s | 60 &mu;s  | 50 &mu;s |
| 48 kHz without file read | 0.5 ms   | x         | 400 &mu;s | 40 &mu;s  | 40 &mu;s |
| 44 kHz with file read    | 3.2 ms   | 2.3 &mu;s | 440 &mu;s | 400 &mu;s | 50 &mu;s |
| 44 kHz without file read | 0.9 ms   | x         | 440 &mu;s | 390 &mu;s | 40 &mu;s |