# Maestro Audio Framework changelog
  ## 2.0.2
  - Removed VoiceSeeker support
  ## 2.0.1
  - Fixed filesrc buffer alignment
  ## 2.0.0 (newest)
  - Added Zephyr port, see [Zephyr README](zephyr/README.rst).
    -  Possible to use standalone version, pulling its own Zephyr and dependencies
    -  Possible to import it as a module in your Zephyr project
  - Changed build system - newly uses Kconfig and Cmake
  - Supports NXP MCUXSDK (previously 2.x)
  - Changed folder structure and names to improve readability (description may be found in [README](README.md))
  - Removed audio libraries and placed into audio-voice-components repository
  - Added libraries are pulled into the build via Kconfig and Cmake
  - Changed Maestro library core - minor changes

  ## 1.8.0
  - New platforms support: MCX-N5XX-EVK, FRDMMCXN236 and RD-RW612-BGA
  - Fixed compilation warnings
  - Documentation improvements and updates
    - Added section with processing time information
    - Added application state diagrams
  - Various updates and fixes
  ## 1.7.0
  - Removed EAP support for future SDK releases
  - Created new API for audio_sink and audio_src to support USB source, sink
  - ASRC library integrated
  - License changed to BSD 3-Clause
  - Improved pipeline creation API
  - Fixed compilation warnings in Opus
  - Various other improvements and bug fixes
  ## 1.6.0
  - Up to 2 parallel pipelines supported
  - Synchronous Sample Rate Converter support Added
  - Various improvements and bug fixes
  ## 1.5.0
  - Enabled switching from 2 to 4 channel output during processing
  - PadReturn type has been replaced by FlowReturn
  - Support of AAC, WAV, FLAC decoders
  - Renamed eap element to audio_proc element
  - Added audio_proc to VIT pipeline to support VoiceSeeker
  - Minor bug fixes
  ## 1.4.0
  - Use Opusfile lib for Ogg Opus decoder
  - Refactor code, fix issues found in unit tests
  - Various bug fixes
  ## 1.3.0
  - Make Maestro framework open source (except mp3 and wav decoder)
  - Refactor code, remove unused parts, add comments
  ## 1.2.0
  - Unified buffering in audio source, audio sink
  - Various improvements and bug fixes
  ## 1.0_rev0
  - Initial version of framework with support for Cortex-M7 platforms
