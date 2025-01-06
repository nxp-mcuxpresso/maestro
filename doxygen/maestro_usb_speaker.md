# Maestro USB speaker example

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
The Maestro USB speaker example demonstrates audio processing on the ARM cortex core utilizing the Maestro Audio Framework library.

The application is controlled by commands from a shell interface using serial console.

The development board will be enumerated as a USB audio class 2.0 device on the USB host. The application takes audio samples from the USB host and sends them to the audio Line-Out port. User will see the volume levels obtained from the USB host but this is only an example application. To leverage the volume values, the demo has to be modified.

Depending on target platform or development board there are different modes and features of the demo supported.
- **Standard** - The mode demonstrates playback with up to 2 channels, up to 48 kHz sample rate and up to 16 bit width. This mode is enabled by default.
- **Multi-Channel** - In this mode the device is enumerated as a UAC 5.1. This mode is disabled by default. See the [Example configuration](#example-configuration) section to see how to enable the mode.
    - When playing an 5.1 audio file, the example sends only the front-left and front-right channels to the audio Line-Out port (the other channels are ignored), since this example only supports on-board codecs with stereo audio output.

As shown in the table below, the application is supported on several development boards, and each development board may have certain limitations, some development boards may also require hardware modifications or allow to use of an audio expansion board. Therefore, please check the supported features and [Hardware modifications](#hardware-modifications) or [Example configuration](#example-configuration) sections before running the demo.

<div class="wy-table-responsive">
<table class="audio_FW_spec_table">
    <thead>
        <tr >
            <th colspan="1">Mode</th>
            <th colspan="2">Standard</th>
            <th colspan="2">Multi-channel</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan="1" rowspan="2" >Feature</td>
            <td colspan="2">Audio output [num of channels]</td>
            <td colspan="2">Audio output [num of channels]</td>
        </tr>
        <tr>
            <td colspan="1">On board codec</td>
            <td colspan="1">aud-exp-42448</td>
            <td colspan="1">On board codec</td>
            <td colspan="1">aud-exp-42448</td>
        </tr>
        <tr>
            <td colspan="1">EVKC-MIMXRT1060</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1">X</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1">X</td>
        </tr>
        <tr>
            <td colspan="1">EVKB-MIMXRT1170</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1" >X</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1" >X</td>
        </tr>
        <tr>
            <td colspan="1">LPCXpresso55s69</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1">X</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1">X</td>
        </tr>
        <tr>
            <td colspan="1">EVK-MCXN5XX</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1">X</td>
            <td colspan="1" class="audio_FW_spec_table_supported">1-2</td>
            <td colspan="1">X</td>
        </tr>
        <tr>
            <td colspan="1">RW612BGA</td>
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
    - If the USB device audio speaker example uses an ISO IN feedback endpoint, please attach the device to a host like PC which supports feedback function. Otherwise, there might be attachment issue or other problems.

**Known issues:**
- No known issues.

More information about supported features can be found on the [Supported features](supported_features.md) page.

## Hardware requirements
- Desired development board
- 2x Micro USB cable
- Personal Computer
- Headphones with 3.5 mm stereo jack

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
4. Connect the second micro USB cable between the PC host and the USB port on the development board.
5. Insert the headphones into Line-Out connector (headphone jack) on the development board.
6. Either press the reset button on your development board or launch the debugger in your IDE to begin running the demo.

## Running the demo
When the example runs successfully, you should see similar output on the serial terminal as below:
```
    **********************************************
    Maestro audio USB speaker solutions demo start
    **********************************************

    Copyright  2022  NXP
    [APP_Shell_Task] start

    >> usb_speaker -1

    Starting maestro usb speaker application
    The application will run until the board restarts
    [STREAMER] Message Task started
    Starting playing
    [STREAMER] start usb speaker
    Set Cur Volume : fbd5
```

Type `help` to see the command list. Similar description will be displayed on serial console:

```
    >> help

    "help": List all the registered commands

    "exit": Exit program

    "version": Display component versions

    "usb_speaker": Play data from the USB port as an audio 2.0
                   speaker device.

      USAGE:      usb_speaker <seconds>
       <seconds>  Time in seconds how long the application should run.
                   When you enter a negative number the application will
                   run until the board restarts.
       EXAMPLE:   The application will run for 20 seconds: usb_speaker 20
```
Details of commands can be found [here](#commands-in-detail).

## Example configuration
The example can be configured by user. Before configuration, please check the [table](#overview) to see if the feature is supported on the development board.
- **Enable Multi-channel mode:**
    - The feature can be enabled by set the `USB_AUDIO_CHANNEL5_1` macro to `1U` in the `usb_device_descriptor.h` file.
    - *Note:* When device functionality is changed, such as UAC 5.1, please uninstall the previous PC driver to make sure the device with changed functionality can run normally.

## Functionality
The `Usb_speaker` command calls the `STREAMER_speaker_Create` function from the `app_streamer.c`file that creates pipeline with the following elements:
    - ELEMENT_USB_SRC_INDEX
    - ELEMENT_SPEAKER_INDEX

Playback itself can be started with the `STREAMER_Start` function.

Each of the elements has several properties that can be accessed using the `streamer_get_property` or `streamer_set_property` function. These properties allow a user to change the values of the appropriate elements. The list of properties can be found in `streamer_element_properties.h`. See the example of setting property value in the following piece of code from the `app_streamer.c` file:

```c
    ELEMENT_PROPERTY_T prop;

    prop.prop = PROP_USB_SRC_SET_SAMPLE_RATE;
    prop.val  = AUDIO_SAMPLING_RATE;

    streamer_set_property(handle->streamer, 0, prop, true);

    prop.prop = PROP_USB_SRC_SET_NUM_CHANNELS;
    prop.val  = 2;

    streamer_set_property(handle->streamer, 0, prop, true);

    prop.prop = PROP_USB_SRC_SET_FRAME_MS;
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
- [usb_speaker \<seconds\>](#usb_speaker-seconds)

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

### usb_speaker \<seconds\>

```{mermaid}
flowchart TD
    classDef function fill:#c6d22c
    classDef condition fill:#7cb2de
    classDef state fill:#fcb415
    classDef error fill:#FF999C

    B((Idle)):::state -->C{Duration
    == 0?}:::condition
    C -- No --> E{Duration
    < 0?}:::condition
    C -- Yes --> D[Error: Incorrect
    command parameter]:::error
    D -->B
    E -- Yes --> G[playing]:::function
    G --> H((Running)):::state
    H --> H
    E -- No --> F[playing]:::function
    F --> I((Running)):::state
    I --> J{Duration
    expired?}:::condition
    J -- No -->I
    J -- Yes --> B
```
