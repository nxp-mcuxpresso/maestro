# Getting started with Maestro framework

## Introduction

This document describes the basic usage of the Maestro Audio Framework. This framework intends to enable the chaining of basic audio processing blocks (called "elements"). These blocks then form stream processing objects ("pipeline").
This pipeline can be used for multiple audio processing use cases.
The processing blocks can include (but are not limited to) different audio sources (for example file or microphone), decoders or encoders, filters or effects, and audio sinks.
For detailed description of the Maestro framework, please refer to the [programmer's guide](doxygen/ProgrammersGuide.md).

Framework overview is depicted in the following picture:

![maestro overview](doxygen/common/maestroApp.svg)

## Supported examples {#supported-examples}

The current version of the Maestro audio framework supports several optional [features](doxygen/supported_features.md), some of which are used in these examples:

- [maestro_playback](doxygen/maestro_playback.md)
- [maestro_record](doxygen/maestro_record.md)
- [maestro_usb_mic](doxygen/maestro_usb_mic.md)
- [maestro_usb_speaker](doxygen/maestro_usb_speaker.md)
- [maestro_sync](doxygen/maestro_sync.md)

The examples can be found in the **audio_examples** folder of the desired board. The demo applications are based on FreeRTOS and use multiple tasks to form the application functionality.

## Example applications overview

To set up the audio framework properly, it is necessary to create a streamer with `streamer_create` API. It is also essential to set up the desired hardware peripherals using the functions described in `streamer_pcm.h`.
The Maestro example projects consist of several files regarding the audio framework. The initial file is `main.c` with code to create multiple tasks.
For features including SD card (in the maestro_playback examples, reading a file from SD card is supported and in maestro_record writing to SD card is currently supported) the `APP_SDCARD_Task` is created.
The command prompt and connected functionalities are handled by `APP_Shell_Task`.

One of the most important parts of the configuration is the `streamer_pcm.c` where the initialization of the hardware peripherals, input and output buffer management can be found. For further information please see also `streamer_pcm.h`

In the Maestro USB examples (maestro_usb_mic and maestro_usb_speaker), the USB configuration is located in the `usb_device_descriptor.c`, `audio_microphone.c` and `audio_speaker.c` files. For further information please see also `usb_device_descriptor.h`, `audio_microphone.h` and `audio_speaker.h`.

In order to be able to get the messages from the audio framework, it is necessary to create a thread for receiving the messages from the streamer, which is usually called a `Message Task`.
The message thread is placed in the `app_streamer.c` file, reads the streamer message queue, and reacts to the following messages:

- STREAM_MSG_ERROR -  stops the streamer and exits the message thread
- STREAM_MSG_EOS - stops the streamer and exits the message thread
- STREAM_MSG_UPDATE_DURATION - prints info about the stream duration
- STREAM_MSG_UPDATE_POSITION - prints info about current stream position
- STREAM_MSG_CLOSE_TASK - exits the message thread

## Configuration options

Users can change the pipeline type when creating the streamer object, please see the [programmer's guide](doxygen/ProgrammersGuide.md).