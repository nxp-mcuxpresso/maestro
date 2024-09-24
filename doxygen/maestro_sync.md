# Maestro sync example

## Table of content
- [Overview](#maestro-sync-overview)
- [Hardware requirements](#maestro-sync-HW-requirements)
- [Hardware modifications](#maestro-sync-HW-modifications)
- [Preparation](#maestro-sync-preparation)
- [Running the demo](#maestro-sync-running-the-demo)
- [Example configuration](#maestro-sync-example-configuration)
- [Functionality](#maestro-sync-functionality)
- [States](#maestro-sync-states)
- [Commands in detail](#maestro-sync-commands-in-detail)

## Overview {#maestro-sync-overview}
The Maestro sync example demonstrates the use of synchronous pipelines (Tx and Rx in this case) processing in a single streamer task on the ARM cortex core utilizing the Maestro Audio Framework library.

The application is controlled by commands from a shell interface using serial console.

The feature is useful for testing the latency of the pipeline or implementing algorithms requiring reference signals such as echo cancellation. The VoiceSeeker library available in this example is not featuring AEC (Acoustic Echo Cancellation), but NXP is offering it in the premium version of the library. More information about the premium version can be found at [VoiceSeeker](https:\\www.nxp.com\voiceseeker) page.
The demo uses two pipelines running synchronously in a single streamer task:
1. Playback (Tx) pipeline:
    - Playback of audio data in PCM format stored in flash memory to the audio Line-Out connector (speaker).
2. Recording (Rx) pipline:
    - Record audio data using a microphone.
    - VoiceSeeker processing.
    - Wake words + voice commands recognition.
    - Save the VoiceSeeker output to the voiceseeker_output.pcm file on the SD card.

As shown in the table below, the application is supported on several development boards, and each development board may have certain limitations, some development boards may also require hardware modifications or allow to use of an audio expansion board. Therefore, please check the supported features and [Hardware modifications](#maestro-sync-HW-modifications) or [Example configuration](#maestro-sync-example-configuration) sections before running the demo.

<table class="audio_FW_spec_table">
    <thead>
        <tr >
            <th colspan="1">Mode</th>
            <th colspan="7">Standard</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan="1" rowspan="2" >Feature</td>
            <td colspan="3">Audio inputs [num of channels]</td>
            <td colspan="2">Audio outputs [num of channels]</td>
            <td colspan="2">Additional libraries</td>
        </tr>
        <tr>
            <td colspan="1">On board codec</td>
            <td colspan="1">DMICs</td>
            <td colspan="1">aud-exp-42448</td>
            <td colspan="1">On board codec</td>
            <td colspan="1">aud-exp-42448</td>
            <td colspan="1">VIT</td>
            <td colspan="1">VoiceSeeker</td>
        </tr>
        <tr>
            <td colspan="1">EVKC-MIMXRT1060</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1</td>
            <td colspan="1">X</td>
            <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
            <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
        </tr>
        <tr>
            <td colspan="1">EVKB-MIMXRT1170</td>
            <td colspan="1">X</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1">X</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1">X</td>
            <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
            <td colspan="1" class="audio_FW_spec_table_limited">OK</td>
        </tr>
        <tr>
            <td colspan="1">LPCXpresso55s69</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
        </tr>
        <tr>
            <td colspan="1">EVK-MCXN5XX</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
            <td colspan="1">X</td>
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
        </tr>
    </tbody>
</table>
- <div style="font-size:.8em"> <span class="audio_FW_spec_table_supported">Dark green</span> - Fully supported and enabled by default.</div>
- <div style="font-size:.8em"> <span class="audio_FW_spec_table_limited">Orange</span> - Supported with some limitations and enabled by default. See the limitations section below.</div>
- <div style="font-size:.8em"> <span class="audio_FW_spec_table_modification">Light green</span> - Supported, but only available after some SW or HW modification. More information about modification can be found in the [Example configuration](#maestro-record-example-configuration) section.</div>
- <div style="font-size:.8em"> <span>X</span> - Not supported.</div>

**Limitations:**
- Addition labraries
    - **VIT:**
        - The VIT is supported only in the MCUXpresso IDE.
    - **VoiceSeeker:**
        - The VoiceSeeker is supported only in the MCUXpresso IDE.

**Known issues:**
- No known issues.

More information about supported features can be found on the [Supported features](supported_features.md) page.

## Hardware requirements {#maestro-sync-HW-requirements}
- Desired development board
- Micro USB cable
- Speaker with 3.5 mm stereo jack
- Personal computer
- Optional:
    - SD card for file output

## Hardware modifications {#maestro-sync-HW-modifications}
Some development boards need some hardware modifications to run the application. If the development board is not listed here, its default setting is required.
- *EVKC-MIMXRT1060:*
    1. Please make sure resistors below are removed to be able to use SD-Card.
        - R368, R347, R349, R365, R363
    2. Please Make sure J99 is installed.

## Preparation {#maestro-sync-preparation}
1. Connect a micro USB cable between the PC host and the debug USB port on the development board
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Insert the speaker into the Line-Out connector (headphone jack) on the development board.
5. *Optional:* Insert an SD card into the SD card slot to record to the VoiceSeeker output.
6. Either press the reset button on your development board or launch the debugger in your IDE to begin running the demo.

## Running the demo {#maestro-sync-running-the-demo}
When the example runs successfully, you should see similar output on the serial terminal as below:
```
    *****************************
    Maestro audio sync demo start
    *****************************

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

    "start [nosdcard]": Starts a streamer task.
                  - Initializes the streamer with the Memory->Speaker pipeline and with
                    the Microphone->VoiceSeeker->VIT->SDcard pipeline.
                  - Runs repeatedly until stop command.
         nosdcard - Doesn't use SD card to store data.

    "stop": Stops a running streamer:

    "debug [on|off]": Starts / stops debugging.
                    - Starts / stops saving VoiceSeeker input data (reference and microphone data)
                      to SDRAM.
                    - After the stop command, this data is transferred to the SD card.
```
Details of commands can be found [here](#maestro-syn-commands-in-detail).

## Example configuration {#maestro-sync-example-configuration}
The example can be configured by user. Before configuration, please check the [table](#maestro-sync-overview) to see if the feature is supported on the development board.
- **Enable the premium version of VoiceSeeker:**
    - The premium version of the VoiceSeeker library with AEC is API compatible with this example.
    - To get the premium version, please visit [VoiceSeeker](https:\\www.nxp.com\voiceseeker) page.
    - The following steps are required to run this example with the VoiceSeeker&AEC library.
        - Link the `voiceseeker.a` library instead of `voiceseeker_no_aec.a`.
        - Set the `RDSP_ENABLE_AEC` definition to `1U` in the `voiceseeker.h` file
- **VIT model generation:**
    - For custom VIT model generation (defining own wake words and voice commands) please use https://vit.nxp.com/

## Functionality {#maestro-sync-functionality}

The `start <nosdcard>` command calls the `STREAMER_Create` function from the `app_streamer.c` file that creates pipelines with the following elements:
- Playback pipeline:
    - ELEMENT_MEM_SRC_INDEX
    - ELEMENT_SPEAKER_INDEX
- Record pipeline:
    - ELEMENT_MICROPHONE_INDEX
    - ELEMENT_VOICESEEKER_INDEX
    - ELEMENT_VIT_PROC_INDEX
    - ELEMENT_FILE_SINK_INDEX (If the `nosdcard` argument is not used)

Processing itself can be started with the `STREAMER_Start` function.

Each of the elements has several properties that can be accessed using the `streamer_get_property` or `streamer_set_property` function. These properties allow a user to change the values of the appropriate elements. The list of properties can be found in `streamer_element_properties.h`. See the example of setting property value in the following piece of code from the `app_streamer.c` file:

```c
    ELEMENT_PROPERTY_T prop;

    MEMSRC_SET_BUFFER_T buf;
    buf.location = (int8_t *)TESTAUDIO_DATA;
    buf.size     = TESTAUDIO_LEN;

    prop.prop = PROP_MEMSRC_SET_BUFF;
    prop.val  = (uintptr_t)&buf;
    if (STREAM_OK != streamer_set_property(handle->streamer, 0, prop, true))
    {
        return kStatus_Fail;
    }

    prop.prop = PROP_MEMSRC_SET_MEM_TYPE;
    prop.val  = AUDIO_DATA;
    if (STREAM_OK != streamer_set_property(handle->streamer, 0, prop, true))
    {
        return kStatus_Fail;
    }

    prop.prop = PROP_MEMSRC_SET_SAMPLE_RATE;
    prop.val  = DEMO_SAMPLE_RATE;
    if (STREAM_OK != streamer_set_property(handle->streamer, 0, prop, true))
    {
        return kStatus_Fail;
    }
```
Some of the predefined values can be found in the `streamer_api.h`.

## States {#maestro-sync-states}
The application can be in 2 different states:
- Idle
- Running

## Commands in detail {#maestro-sync-commands-in-detail}
- [help, version](#maestro-sync-help-version)
- [start [nosdcard]](#maestro-sync-start)
- [stop](#maestro-sync-stop)
- [debug [on|off]](#maestro-sync-debug-on-off)

Legend for diagrams:
@mermaid{legend}

### help, version {#maestro-sync-help-version}

@mermaid{help_version2}

### start [nosdcard] {#maestro-sync-start}

@mermaid{maestro-sync-start}

### stop {#maestro-sync-stop}

@mermaid{maestro-sync-stop}

### debug [on|off] {#maestro-sync-debug-on-off}

@mermaid{maestro-sync-debug-on-off}