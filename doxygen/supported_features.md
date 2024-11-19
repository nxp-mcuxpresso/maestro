# Supported features

The current version of the audio framework supports several optional features. These can be limited to some MCU cores or development boards variants. More information about support can be found on the specific example page:
- [maestro_playback](maestro_playback.md)
- [maestro_record](maestro_record.md)
- [maestro_usb_mic](maestro_usb_mic.md)
- [maestro_usb_speaker](maestro_usb_speaker.md)
- [maestro_sync](maestro_sync.md)

Some features are delivered as prebuilt library and the binaries can be found in the `\middleware\audio_voice\components\*component*\libs` folder. The source code of some features can be found in the `\middleware\audio_voice\maestro\src` folder.

## Decoders

Supported decoders and its options are:

| Decoder               | Sample rates [kHz]                                            | Number of channels                | Bit depth                 |
|-----------------------|---------------------------------------------------------------|-----------------------------------|---------------------------|
| AAC                   | 8, 11.025, 12, 16, 22.05, 24, 32, 44.1, 48                       | 1, 2 (mono/stereo)                | 16                        |
| FLAC                  | 8, 11.025, 12, 16, 22.05, 32, 44.1, 48                       | 1, 2 (mono/stereo)                | 16                        |
| MP3                   | 8, 11.025, 12, 16, 22.05, 24, 32, 44.1, 48                           | 1, 2 (mono/stereo)                | 16                        |
| OPUS                  | 8, 16, 24, 48                                                 | 1, 2 (mono/stereo)                | 16                        |
| WAV                   | 8, 11.025, 16, 22.05, 32, 44.1, 48                           | 1, 2 (mono/stereo)                | 8, 16, 24                 |

For more details about the reference decoders please see audio-voice-components repository documentation `\middleware\audio_voice\components\`.

## Encoders

- **OPUS encoder** - The current verion of the audio framework only supports a OPUS encoder. For more details about the encoder please see the following [link](https://opus-codec.org/docs/opus_api-1.3.1/).

## Sample rate converters

- **SSRC** - Synchronous sample rate converter. More details about SSRC are available in the User Guide, which is located in `middleware\audio_voice\components\ssrc\doc\`.

- **ASRC** - Asynchronous sample rate converter is not used in our examples, but it is part of the maestro middleware and can be enabled. To enable ASRC, the `maestro_framework_asrc` and `CMSIS_DSP_Library_Source` components must be added to the project. Furthermore, it is necessary to switch from Redlib to Newlib (semihost) library and add a platform definition to the project (e.g. for RT1170: `PLATFORM_RT1170_CORTEXM7`). Supported platforms can be found in the `PL_platformTypes.h` file. More details about ASRC are available in the User Guide, which is located in `middleware\audio_voice\components\asrc\doc\`.

## Additional libraries

- **VIT** - Voice Intelligent Technology (VIT) Wake Word and Voice Command Engines provide free, ready to use voice UI enablement for developers. It enables customer-defined wake words and commands using free online tools. More details about VIT are available in the VIT package, which is located in `middleware\audio_voice\components\vit\{platform}\Doc\`(depending on the platform) or via following [link](https://nxp.com/vit).

- **VoiceSeeker** - VoiceSeeker is a multi-microphone voice control audio front-end signal processing solution. More details about VoiceSeeker are available in the VoiceSeeker package, which is located in `middleware\audio_voice\components\voice_seeker\{platform}\Doc\`(depending on the platform) or via following [link](https://nxp.com/voiceseeker).
