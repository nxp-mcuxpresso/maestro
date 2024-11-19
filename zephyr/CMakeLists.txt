# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MAESTRO_AUDIO_FRAMEWORK)

    project(Maestro C)

    # ============================================
    # Set varriables and project target
    # ============================================

    # Set path varriables
    set(MAESTRO_DIR ${ZEPHYR_CURRENT_MODULE_DIR})

    set(MAESTRO_API_DIR ${MAESTRO_DIR}/src/inc)
    set(MAESTRO_CORE_DIR ${MAESTRO_DIR}/src/core)
    set(MAESTRO_ELEMENTS_DIR ${MAESTRO_DIR}/src/elements)
    set(MAESTRO_DEVICES_DIR ${MAESTRO_DIR}/src/devices)
    set(MAESTRO_UTILS_DIR ${MAESTRO_DIR}/src/utils)
    set(MAESTRO_COMPONENTS_GLUE_DIR ${MAESTRO_DIR}/components)
    set(MAESTRO_COMPONENTS_DECODERS_DIR ${MAESTRO_DIR}/components/decoders)
    set(MAESTRO_COMPONENTS_ENCODERS_DIR ${MAESTRO_DIR}/components/encoders)
    set(MAESTRO_CCI_DIR ${MAESTRO_DIR}/src/cci)
    set(MAESTRO_CEI_DIR ${MAESTRO_DIR}/src/cei)

    set(MAESTRO_COMPONENTS_REPO_DIR ${MAESTRO_DIR}/../maestro_components)
    set(MAESTRO_WRAPPERS_DIR ${MAESTRO_DIR}/zephyr/wrappers)
    set(MAESTRO_OSA_DIR ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/osa)

    # message("Maestro path: ${MAESTRO_DIR}")
    # message("Maestro API path: ${MAESTRO_API_DIR}")
    # message("Maestro core Path: ${MAESTRO_CORE_DIR}")
    # message("Maestro elements Path: ${MAESTRO_ELEMENTS_DIR}")
    # message("Maestro devices Path: ${MAESTRO_DEVICES_DIR}")
    # message("Maestro utils Path: ${MAESTRO_UTILS_DIR}")
    # message("Maestro components glue Path: ${MAESTRO_COMPONENTS_GLUE_DIR}")
    # message("Maestro components repos Path: ${MAESTRO_COMPONENTS_REPO_DIR}")
    # message("Maestro wrappers Path: ${MAESTRO_WRAPPERS_DIR}")
    # message("Maestro OSA Path: ${MAESTRO_OSA_DIR}")

    # Set library target
    set(MAESTRO_LIB Maestro_audio_framework)
    zephyr_library_named(${MAESTRO_LIB})

    # ============================================
    # Include Maestro library into the build
    # ============================================
    message("Including Maestro library into the build.")

    file(GLOB MAESTRO_API_FILES "${MAESTRO_API_DIR}/*.h")
    file(GLOB MAESTRO_CORE_H_FILES "${MAESTRO_CORE_DIR}/inc/*.h")
    file(GLOB MAESTRO_CORE_C_FILES "${MAESTRO_CORE_DIR}/src/*.c")

    zephyr_include_directories(
        ${MAESTRO_API_DIR}
        ${MAESTRO_CORE_DIR}/inc
        ${MAESTRO_UTILS_DIR}/inc
        ${MAESTRO_ELEMENTS_DIR}
        ${MAESTRO_DEVICES_DIR}
    )

    zephyr_library_sources(
        ${MAESTRO_API_FILES}
        ${MAESTRO_CORE_H_FILES}
        ${MAESTRO_CORE_C_FILES}
        ${MAESTRO_UTILS_DIR}/inc/general_utils.h
        ${MAESTRO_UTILS_DIR}/inc/maestro_logging.h
        ${MAESTRO_UTILS_DIR}/src/general_utils.c
        ${MAESTRO_UTILS_DIR}/src/maestro_logging.c
    )

    # ============================================
    # Include Maestro CCI and CEI into the build
    # ============================================

    if(CONFIG_MAESTRO_CEI_ENABLE)
        zephyr_include_directories(${MAESTRO_CEI_DIR}/inc/)
    endif()

    if(CONFIG_MAESTRO_CCI_ENABLE)
        file(GLOB CCI_API_SOURCES "${MAESTRO_CCI_DIR}/src/*.c")
        file(GLOB CCI_PARSERS_SOURCES "${MAESTRO_CCI_DIR}/../parsers/cci/*.c")

        zephyr_include_directories(
            ${MAESTRO_CCI_DIR}/inc/
            ${MAESTRO_CCI_DIR}/parsers/cci/
        )

        zephyr_library_sources(
            ${CCI_API_SOURCES}
            ${CCI_PARSERS_SOURCES}
        )
    endif() # CONFIG_MAESTRO_CCI_ENABLE

    # ============================================
    # Include Maestro elements into the build
    # ============================================

    if(CONFIG_MAESTRO_ELEMENT_MEM_SINK_ENABLE)
        zephyr_library_sources(
            ${MAESTRO_ELEMENTS_DIR}/mem_sink.c
            ${MAESTRO_ELEMENTS_DIR}/mem_sink.h
            )
        zephyr_compile_definitions("-DSTREAMER_ENABLE_MEM_SINK")
    endif()

    if(CONFIG_MAESTRO_ELEMENT_MEM_SRC_ENABLE)
        zephyr_library_sources(
            ${MAESTRO_ELEMENTS_DIR}/mem_src.c
            ${MAESTRO_ELEMENTS_DIR}/mem_src.h
            )
        zephyr_compile_definitions("-DSTREAMER_ENABLE_MEM_SRC")
    endif()

    if(CONFIG_MAESTRO_ELEMENT_AUDIO_SRC_ENABLE)
        zephyr_library_sources(
            ${MAESTRO_ELEMENTS_DIR}/audio_src.c
            ${MAESTRO_ELEMENTS_DIR}/audio_src.h
            ${MAESTRO_DEVICES_DIR}/audio_src_default.c
            ${MAESTRO_DEVICES_DIR}/audio_src_default.h
            )
        zephyr_compile_definitions("-DSTREAMER_ENABLE_AUDIO_SRC")
    endif()

    if(CONFIG_MAESTRO_ELEMENT_AUDIO_SINK_ENABLE)
        zephyr_library_sources(
            ${MAESTRO_ELEMENTS_DIR}/audio_sink.c
            ${MAESTRO_ELEMENTS_DIR}/audio_sink.h
            ${MAESTRO_DEVICES_DIR}/audio_sink_default.c
            ${MAESTRO_DEVICES_DIR}/audio_sink_default.h
            )
        zephyr_compile_definitions("-DSTREAMER_ENABLE_AUDIO_SINK")
    endif()

    if(CONFIG_MAESTRO_ELEMENT_PROCESS_SINK_ENABLE)
        zephyr_library_sources(
            ${MAESTRO_ELEMENTS_DIR}/process_sink.c
            ${MAESTRO_ELEMENTS_DIR}/process_sink.h
            )
        zephyr_compile_definitions("-DSTREAMER_ENABLE_PROCESS_SINK")
    endif()

    if(CONFIG_MAESTRO_ELEMENT_DECODER_ENABLE)
        zephyr_library_sources(
            ${MAESTRO_ELEMENTS_DIR}/decoder.c
            ${MAESTRO_ELEMENTS_DIR}/decoder.h
            ${MAESTRO_ELEMENTS_DIR}/decoder_pads.c
            ${MAESTRO_ELEMENTS_DIR}/decoder_pads.h
            )
        zephyr_compile_definitions("-DSTREAMER_ENABLE_DECODER")
    endif()

    if(CONFIG_MAESTRO_ELEMENT_ENCODER_ENABLE)
        zephyr_library_sources(
            ${MAESTRO_ELEMENTS_DIR}/encoder.c
            ${MAESTRO_ELEMENTS_DIR}/encoder.h
            )
        zephyr_compile_definitions("-DSTREAMER_ENABLE_ENCODER")
    endif()

    if(CONFIG_MAESTRO_ELEMENT_AUDIO_PROC_ENABLE)
        message(SEND_ERROR "Maestro Audio proc element not implemented yet. TODO")
    endif()

    if(CONFIG_MAESTRO_ELEMENT_VOICE_SEEKER_ENABLE)
        message(SEND_ERROR "Maestro Voice Seeker element not implemented yet. TODO")
    endif()

    if(CONFIG_MAESTRO_ELEMENT_FILE_SINK_ENABLE)
        message(SEND_ERROR "Maestro File sink element not implemented yet. TODO")
    endif()

    if(CONFIG_MAESTRO_ELEMENT_FILE_SRC_ENABLE)
        message(SEND_ERROR "Maestro File source element not implemented yet. TODO")
    endif()

    if(CONFIG_MAESTRO_ELEMENT_NETBUF_SRC_ENABLE)
        message(SEND_ERROR "Maestro Netbuf source element not implemented yet. TODO")
    endif()

    # ============================================
    # Include Maestro Decoders into the build
    # ============================================

    if(CONFIG_MAESTRO_ELEMENT_DECODER_MP3_ENABLE)
        message("Including Maestro decoder MP3 into the build")
        file(GLOB DECODER_MP3_SOURCES ${MAESTRO_COMPONENTS_DECODERS_DIR}/mp3/*.c)

        zephyr_library_sources(${DECODER_MP3_SOURCES})
        zephyr_include_directories(${MAESTRO_COMPONENTS_DECODERS_DIR}/mp3/)
        zephyr_library_compile_definitions(MP3_DEC=1)
    endif()

    if(CONFIG_MAESTRO_ELEMENT_DECODER_WAV_ENABLE)
        message("Including Maestro decoder WAV into the build")
        file(GLOB DECODER_WAV_SOURCES ${MAESTRO_COMPONENTS_DECODERS_DIR}/wav/*.c)

        zephyr_library_sources(${DECODER_WAV_SOURCES})
        zephyr_include_directories(${MAESTRO_COMPONENTS_DECODERS_DIR}/wav/)
        zephyr_library_compile_definitions(WAV_DEC=1)
    endif()

    if(CONFIG_MAESTRO_ELEMENT_DECODER_FLAC_ENABLE)
        message("Including Maestro decoder FLAC into the build")
        file(GLOB DECODER_FLAC_SOURCES ${MAESTRO_COMPONENTS_DECODERS_DIR}/flac/*.c)

        zephyr_library_sources(${DECODER_FLAC_SOURCES})
        zephyr_library_compile_definitions(FLAC_DEC=1)

    endif()

    if(CONFIG_MAESTRO_ELEMENT_DECODER_AAC_ENABLE)
        message("Including Maestro decoder AAC into the build")
        file(GLOB DECODER_AAC_SOURCES ${MAESTRO_COMPONENTS_DECODERS_DIR}/aac/*.c)

        zephyr_library_sources(${DECODER_AAC_SOURCES})
        zephyr_library_compile_definitions(AAC_DEC=1)
    endif()

    if(CONFIG_MAESTRO_ELEMENT_DECODER_OPUS_OGG_ENABLE)
        message("Including Maestro decoder OPUS OGG into the build")

        zephyr_library_sources(${MAESTRO_COMPONENTS_DECODERS_DIR}/opus/opus_extractmetadata.c)
        zephyr_library_compile_definitions(OGG_OPUS_DEC=1)
    endif()

    if(CONFIG_MAESTRO_ELEMENT_DECODER_OPUS_ENABLE)
        message(SEND_ERROR "Maestro Decoder OPUS not implemented yet. TODO")

        # zephyr_library_sources(${MAESTRO_COMPONENTS_DECODERS_DIR}/opus/opus_extractmetadata.c)
        # zephyr_library_compile_definitions(OPUS_DEC=1)
        # message("Including Maestro decoder OPUS into the build")
    endif()

    # ============================================
    # Include Maestro Encoders into the build
    # ============================================
    if(CONFIG_MAESTRO_ELEMENT_ENCODER_OPUS_OGG_ENABLE)
        message(SEND_ERROR "Maestro encoder OPUS OGG not implemented yet. TODO")
        # message("Including Maestro encoder OPUS OGG into the build")

        # zephyr_library_sources(${MAESTRO_COMPONENTS_ENCODERS_DIR}/opus/opusenc_cei.c)
        # zephyr_include_directories(${MAESTRO_COMPONENTS_ENCODERS_DIR}/opus/)

        # zephyr_library_compile_definitions(OPUS_DEC=1)
        # zephyr_library_compile_definitions(STREAMER_ENABLE_CEI_OPUS=1)
        # zephyr_library_compile_definitions(OPUS_ENCODE)
    endif()

    if(CONFIG_MAESTRO_ELEMENT_ENCODER_OPUS_ENABLE)
        # message(SEND_ERROR "Maestro encoder OPUS not implemented yet. TODO")
        message("Including Maestro encoder OPUS into the build")

        zephyr_library_sources(${MAESTRO_COMPONENTS_ENCODERS_DIR}/opus/opusenc_cei.c)
        zephyr_include_directories(${MAESTRO_COMPONENTS_ENCODERS_DIR}/opus/)

        zephyr_library_compile_definitions(STREAMER_ENABLE_CEI_OPUS=1)
        zephyr_library_compile_definitions(OPUS_ENCODE)
    endif()

    # ============================================
    # Include SDK Wrappers into the build
    # ============================================
    zephyr_include_directories(${MAESTRO_WRAPPERS_DIR})

    # ============================================
    # Include Zephyr OSA into the build
    # ============================================
    # if(CONFIG_MAESTRO_OSA_USE)
    #   add_subdirectory_ifdef(CONFIG_MAESTRO_OSA_USE ${MAESTRO_DIR}/zephyr/osa)
    # else()
    #   message(FATAL_ERROR "!!!! OSA not in use !!!!\r\n")
    # endif()
    # add_compile_definitions(FSL_OSA_TASK_ENABLE=1)

    # add_subdirectory(${MAESTRO_OSA_DIR})

    # ============================================
    # Include OSA layer into the build
    # ============================================
    # set(CONFIG_USE_component_osa_zephyr true PARENT_SCOPE)
    # set(CONFIG_NXP_OSA true PARENT_SCOPE)
    # set(CONFIG_NXP_OSA_INCLUDE_TASK_API true PARENT_SCOPE)
    add_compile_definitions(FSL_OSA_TASK_ENABLE=1)
    set(CONFIG_USE_component_osa_zephyr true)

    zephyr_include_directories(${MAESTRO_OSA_DIR})
    # zephyr_library_sources(${MAESTRO_OSA_DIR}/fsl_os_abstraction_zephyr.c)

else() # CONFIG_MAESTRO_AUDIO_FRAMEWORK=n
    message("Maestro library excluded from the build. ")

endif()
