# Maestro USB microphone example

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

## Overview
The Maestro USB microphone example demonstrates audio processing on the ARM cortex core utilizing the Maestro Audio Framework library.

The application is controlled by commands from a shell interface using serial console.

The development board will be enumerated as a USB audio class 2.0 device on the USB host. The application takes audio samples from the microphone inputs and sends them to the USB host via the USB bus. User will see the volume levels obtained from the USB host but this is only an example application. To leverage the volume values, the demo has to be modified.

As shown in the table below, the application is supported on several development boards, and each development board may have certain limitations, some development boards may also require hardware modifications or allow to use of an audio expansion board. Therefore, please check the supported features and [Hardware modifications](#hardware-modifications) or [Example configuration](#example-configuration) sections before running the demo.

<div class="wy-table-responsive">
    <table class="audio_FW_spec_table">
        <thead>
            <tr >
                <th colspan="1">Mode</th>
                <th colspan="3">Standard</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td colspan="1" rowspan="2" >Feature</td>
                <td colspan="3">Audio input [num of channels]</td>
            </tr>
            <tr>
                <td colspan="1">On board codec</td>
                <td colspan="1">DMICs</td>
                <td colspan="1">aud-exp-42448</td>
            </tr>
            <tr>
                <td colspan="1">EVKC-MIMXRT1060</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
            </tr>
            <tr>
                <td colspan="1">EVKB-MIMXRT1170</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1</td>
                <td colspan="1" >X</td>
                <td colspan="1" >X</td>
            </tr>
            <tr>
                <td colspan="1">LPCXpresso55s69</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
            </tr>
            <tr>
                <td colspan="1">EVK-MCXN5XX</td>
                <td colspan="1" class="audio_FW_spec_table_supported">1</td>
                <td colspan="1">X</td>
                <td colspan="1">X</td>
            </tr>
            <tr>
                <td colspan="1">RW612BGA</td>
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
- *Note:*
    1. When connected to MacBook, change the PCM format from (0x02,0x00,) to (0x01,0x00, ) in the g_config_descriptor[CONFIG_DESC_SIZE] in the `usb_descriptor.c` file. Otherwise, it can't be enumerated and noise is present when recording with the QuickTime player because the sampling frequency and bit resolution do not match.
    2. When device functionality is changed, please uninstall the previous PC driver to make sure the device with changed functionality can run normally.
    3. If you're having audio problems on Windows 10 for recorder, please disable signal enhancement as the following if it is enabled and have a try again.

**Known issues:**
- No  known issues.

More information about supported features can be found on the [Supported features](supported_features.md) page.

## Hardware requirements
- Desired development board
- 2x Micro USB cable
- Personal Computer
- *LPCXpresso55s69:*
    - Source of sound with 3.5 mm stereo jack connector

## Hardware modifications
Some development boards need some hardware modifications to run the application. If the development board is not listed here, its default setting is required.

## Preparation
1. Connect the first micro USB cable between the PC host and the debug USB port on the development board
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. *LPCXpresso55s69:*
    - Insert source of sound to Audio Line-In connector (headphone jack) on the development board.
5. Connect the second micro USB cable between the PC host and the USB port on the development board.
6. Either press the reset button on your development board or launch the debugger in your IDE to begin running the demo.

## Running the demo
When the example runs successfully, you should see similar output on the serial terminal as below:
```
    *************************************************
    Maestro audio USB microphone solutions demo start
    *************************************************

    Copyright  2022  NXP
    [APP_Shell_Task] start

    >> usb_mic -1

    Starting maestro usb microphone application
    The application will run until the board restarts
    [STREAMER] Message Task started
    Starting recording
    [STREAMER] start usb microphone
    Set Cur Volume : 1f00
```

Type `help` to see the command list. Similar description will be displayed on serial console:

```
    >> help

    "help": List all the registered commands

    "exit": Exit program

    "version": Display component versions

    "usb_mic":  Record MIC audio and playback to the USB port as an audio 2.0
                microphone device.

      USAGE: usb_mic <seconds>
        <seconds>  Time in seconds how long the application should run.
                   When you enter a negative number the application will
                   run until the board restarts.
      EXAMPLE:   The application will run for 20 seconds: usb_mic 20
```
Details of commands can be found [here](#commands-in-detail).

## Example configuration
The example only supports one mode and do not support any additional libraries, so the example can't be configured by user.

## Functionality
The `usb_mic` command calls the `STREAMER_mic_Create` function from the `app_streamer.c`file that creates pipeline with the following elements:
    - ELEMENT_MICROPHONE_INDEX
    - ELEMENT_USB_SINK_INDEX

Recording itself can be started with the `STREAMER_Start` function.

Each of the elements has several properties that can be accessed using the `streamer_get_property` or `streamer_set_property` function. These properties allow a user to change the values of the appropriate elements. The list of properties can be found in `streamer_element_properties.h`. See the example of setting property value in the following piece of code from the `app_streamer.c` file:

```c
    ELEMENT_PROPERTY_T prop;

    prop.prop = PROP_MICROPHONE_SET_SAMPLE_RATE;
    prop.val  = AUDIO_SAMPLING_RATE;

    streamer_set_property(handle->streamer, 0, prop, true);

    prop.prop = PROP_MICROPHONE_SET_NUM_CHANNELS;
    prop.val  = 1;

    streamer_set_property(handle->streamer, 0, prop, true);

    prop.prop = PROP_MICROPHONE_SET_FRAME_MS;
    prop.val  = 1;

    streamer_set_property(handle->streamer, 0, prop, true);
```
Some of the predefined values can be found in the `streamer_api.h`.

## States
The application can be in 2 different states:
- Idle
- Running

## Commands in detail
- [help, version](#help-version)
- [usb_mic \<seconds\>](#usb_mic-seconds)

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

### usb_mic \<seconds\>

```{mermaid}
flowchart TD
    classDef function fill:#c6d22c
    classDef condition fill:#7cb2de
    classDef state fill:#fcb415
    classDef error fill:#FF999C

    B((Idle)):::state -->C{seconds
    == 0?}:::condition
    C -- No --> E{seconds
    < 0?}:::condition
    C -- Yes --> D[Error: Incorrect
    command parameter]:::error
    D -->B
    E -- Yes --> G[recording]:::function
    G --> H((Running)):::state
    H --> H
    E -- No --> F[recording]:::function
    F --> I((Running)):::state
    I --> J{seconds
    expired?}:::condition
    J -- No -->I
    J -- Yes --> B
```