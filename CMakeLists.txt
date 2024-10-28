if (CONFIG_MCUX_COMPONENT_middleware.audio_voice.maestro)
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src
        INCLUDES inc
                 core/inc
                 utils/inc
                 elements
                 devices
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/middleware/audio_voice/maestro/src
        SOURCES inc/*.h
                core/inc/*.h
                core/src/*.c
                utils/inc/general_utils.h
                utils/inc/maestro_logging.h
                utils/src/general_utils.c
                utils/src/maestro_logging.c
    )
endif()

include(${SdkRootDirPath}/middleware/audio_voice/maestro/components.cmake OPTIONAL)
include(${SdkRootDirPath}/middleware/audio_voice/maestro/configs.cmake OPTIONAL)
include(${SdkRootDirPath}/middleware/audio_voice/maestro/elements.cmake OPTIONAL)
