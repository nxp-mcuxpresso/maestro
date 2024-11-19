# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.decoder.aac.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/components/decoders/aac
        SOURCES aac_extractmetadata.c
        TOOLCHAINS armgcc mcux
    )

    mcux_add_macro(
        CC "-DAAC_DEC=1"
        TOOLCHAINS armgcc mcux
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.decoder.flac.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/components/decoders/flac
        SOURCES flac_extractmetadata.c
        TOOLCHAINS armgcc iar mcux
    )

    mcux_add_macro(
        CC "-DFLAC_DEC=1"
        TOOLCHAINS armgcc iar mcux
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.decoder.mp3.enable)
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/components/decoders
        INCLUDES mp3
        TOOLCHAINS armgcc iar mcux
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/components/decoders
        SOURCES mp3/*.h
                mp3/*.c
        TOOLCHAINS armgcc iar mcux
    )

    mcux_add_macro(
        CC "-DMP3_DEC=1"
        TOOLCHAINS armgcc iar mcux
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.decoder.wav.enable)
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/components/decoders
        INCLUDES wav
        TOOLCHAINS armgcc iar mcux
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/components/decoders
        SOURCES wav/*.h
                wav/*.c
        TOOLCHAINS armgcc iar mcux
    )

    mcux_add_macro(
        CC "-DWAV_DEC=1"
        TOOLCHAINS armgcc iar mcux
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.decoder.opus_ogg.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/components/decoders
        SOURCES opus/opus_extractmetadata.c
    )
    mcux_add_macro(
        CC "-DOGG_OPUS_DEC=1"
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.encoder.opus.enable)
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/components/encoders
        INCLUDES opus
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/components/encoders
        SOURCES opus/opusenc_cei.c
                opus/opusenc_cei.h
    )

    mcux_add_macro(
        CC "-DOPUS_ENCODE=1\
            -DSTREAMER_ENABLE_CEI_OPUS=1"
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.audio_proc.asrc.enable)
    mcux_add_macro(
        CC "-DASRC_PROC=1"
        TOOLCHAINS armgcc mcux
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.audio_proc.ssrc.enable)
    mcux_add_macro(
        CC "-DSSRC_PROC=1"
        TOOLCHAINS armgcc iar mcux
    )
endif()

if (CONFIG_MCUX_PRJSEG_middleware.audio_voice.components.voice_seeker)
    mcux_add_macro(
        CC "-DVOICE_SEEKER_PROC=1"
        TOOLCHAINS armgcc mcux
    )
endif()

if (CONFIG_MCUX_PRJSEG_middleware.audio_voice.components.vit)
    mcux_add_macro(
        CC "-DVIT_PROC=1"
        TOOLCHAINS armgcc mcux
    )
endif()