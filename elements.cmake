if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.audio_proc.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/elements
        SOURCES audio_proc.c
                audio_proc.h
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.audio_sink.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/elements
        SOURCES audio_sink.c
                audio_sink.h
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/devices
        SOURCES audio_sink_default.c
                audio_sink_default.h
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.audio_src.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/elements
        SOURCES audio_src.c
                audio_src.h
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/devices
        SOURCES audio_src_default.c
                audio_src_default.h
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.file_utils.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/utils
        SOURCES inc/file_utils.h
                src/file_utils.c
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.file_sink.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/elements
        SOURCES file_sink.c
                file_sink.h
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.file_src.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/elements
        SOURCES file_src.c
                file_src.h
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.mem_sink.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/elements
        SOURCES mem_sink.c
                mem_sink.h
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.mem_src.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/elements
        SOURCES mem_src.c
                mem_src.h
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.netbuf_src.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/elements
        SOURCES netbuf_src.c
                netbuf_src.h
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.process_sink.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/elements
        SOURCES process_sink.c
                process_sink.h
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.cci.enable)
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src
        INCLUDES cci/inc
                 parsers/cci
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src
        SOURCES cci/inc/*.h
                cci/src/*.c
                parsers/cci/*.c
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.cei.enable)
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src
        INCLUDES cei/inc
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src
        SOURCES cei/inc/*.h
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.decoder.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/elements
        SOURCES decoder.c
                decoder.h
                decoder_pads.c
                decoder_pads.h
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro.element.encoder.enable)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src/elements
        SOURCES encoder.c
                encoder.h
    )
endif()
