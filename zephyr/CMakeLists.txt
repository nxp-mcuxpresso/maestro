if(CONFIG_MAESTRO_AUDIO_FRAMEWORK)

  project (Maestro C)

  # Set paths
  set(MAESTRO_DIR ${ZEPHYR_CURRENT_MODULE_DIR})
  set(MAESTRO_WRAPPERS_DIR ${MAESTRO_DIR}/zephyr/wrappers)
  set(MAESTRO_COMPONENTS_DIR ${MAESTRO_DIR}/components)
  set(MAESTRO_OSA_DIR ${ZEPHYR_HAL_NXP_MODULE_DIR}/mcux/mcux-sdk/components/osa)

  message("Maestro Path: ${MAESTRO_DIR}")
  message("Maestro Wrappers Path: ${MAESTRO_WRAPPERS_DIR}")
  message("Maestro Components Path: ${MAESTRO_COMPONENTS_DIR}")
  message("Maestro OSA Path: ${MAESTRO_OSA_DIR}")

  set(MAESTRO_LIB Maestro_audio_framework)
  zephyr_library_named(${MAESTRO_LIB})

  # ============================================
  # Include Maestro library into the build
  # ============================================
  message("Including Maestro library into the build.")
  zephyr_library_sources(
    ${MAESTRO_DIR}/streamer/core/pad.c
    ${MAESTRO_DIR}/streamer/core/pipeline.c
    ${MAESTRO_DIR}/streamer/core/streamer.c
    ${MAESTRO_DIR}/streamer/core/streamer_element.c
    ${MAESTRO_DIR}/streamer/core/streamer_msg.c
    #${MAESTRO_DIR}/streamer/decoders/cci_dec/ccidec.c
    #${MAESTRO_DIR}/streamer/decoders/cci_dec/codec_interface.c
    #${MAESTRO_DIR}/streamer/decoders/cci_dec/codecextractmetadata.c
    #${MAESTRO_DIR}/streamer/devices/audio_sink_pcmrtos.c
    ${MAESTRO_DIR}/streamer/devices/audio_src_pcmrtos.c
    #${MAESTRO_DIR}/streamer/elements/audio_proc.c
    #${MAESTRO_DIR}/streamer/elements/audio_sink.c
    ${MAESTRO_DIR}/streamer/elements/audio_src.c
    #${MAESTRO_DIR}/streamer/elements/decoder.c
    #${MAESTRO_DIR}/streamer/elements/decoder_pads.c
    #${MAESTRO_DIR}/streamer/elements/file_sink.c
    #${MAESTRO_DIR}/streamer/elements/file_src_freertos.c
    ${MAESTRO_DIR}/streamer/elements/mem_sink.c
    ${MAESTRO_DIR}/streamer/elements/vit_sink.c
    ${MAESTRO_DIR}/streamer/elements/mem_src.c
    #${MAESTRO_DIR}/streamer/elements/netbuf_src.c
    #${MAESTRO_DIR}/streamer/parsers/cci/cci_codec_type_conversion.c
    #${MAESTRO_DIR}/streamer/elements/encoder.c
    #${MAESTRO_DIR}/streamer/encoders/opus/opusenc_cei.c
    #${MAESTRO_DIR}/streamer/cci/metadata/src/mp3/mp3_extractmetadata.c
    #${MAESTRO_DIR}/streamer/cci/metadata/src/mp3/mp3_id3v2.c
    #${MAESTRO_DIR}/streamer/cci/metadata/src/mp3/mp3_vbr.c
    #${MAESTRO_DIR}/streamer/cci/metadata/src/adpcm/adpcm_extractmetadata.c
    #${MAESTRO_DIR}/streamer/cci/metadata/src/adpcm/wav_common.c
    #${MAESTRO_DIR}/streamer/cci/metadata/src/opus/opus_extractmetadata.c
    #${MAESTRO_DIR}/streamer/cci/metadata/src/aac/aac_extractmetadata.c
    #${MAESTRO_DIR}/streamer/cci/metadata/src/flac/flac_extractmetadata.c
    ${MAESTRO_DIR}/streamer/utils/file_utils.c
    ${MAESTRO_DIR}/streamer/utils/general_utils.c
    ${MAESTRO_DIR}/streamer/utils/maestro_logging.c
    ${MAESTRO_DIR}/streamer/utils/ringbuffer.c
    )
  zephyr_include_directories(
    ${MAESTRO_DIR}/config
    ${MAESTRO_DIR}/inc
    #${MAESTRO_DIR}/streamer/cci/include
    ${MAESTRO_DIR}/streamer/core
    #${MAESTRO_DIR}/streamer/decoders/cci_dec
    ${MAESTRO_DIR}/streamer/devices
    ${MAESTRO_DIR}/streamer/elements
    #${MAESTRO_DIR}/streamer/encoders/opus
    #${MAESTRO_DIR}/streamer/encoders/cei
    )

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
  # Include SDK Wrappers into the build
  # ============================================
  zephyr_include_directories(${MAESTRO_WRAPPERS_DIR})

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

  # ============================================
  # Include Maestro's components into the build
  # ============================================
  # Include VIT into the build (For now, linking only for Cortex M33, RW612)
  # TBD: All Languages
  # TBD: Based on specific platforms. Abstract the path
  if(CONFIG_MAESTRO_ENABLE_ELEMENT_VIT_SINK)
    # if(CONFIG_MAESTRO_VIT_LINKING_ISSUE_DOUBLE_DEFINITION)
    #   set (CMAKE_VIT_LINKER_OPTIONS "-Wl,-z,multidefs")
    #   set (CMAKE_VIT_LINKER_OPTIONS "-Wl,--allow-multiple-definition")
    #   target_link_options(app PUBLIC ${CMAKE_VIT_LINKER_OPTIONS})
    # endif()

    if(CONFIG_BOARD_RD_RW612_BGA AND CONFIG_BOARD_RD_RW612_BGA_RW612)
      #add_subdirectory_ifdef(CONFIG_MAESTRO_ENABLE_ELEMENT_VIT_SINK ${MAESTRO_COMPONENTS_DIR}/vit)
      set(MAESTRO_COMPONENT_VIT_DIR ${MAESTRO_COMPONENTS_DIR}/vit)
      set(MAESTRO_COMPONENT_VIT_LIB_PATH ${MAESTRO_COMPONENT_VIT_DIR}/CortexM33-RW61X/Lib/libVIT_CM33_RW61X_v04_10_00_test.a)
      zephyr_library_import(vit_lib ${MAESTRO_COMPONENT_VIT_LIB_PATH})

      zephyr_include_directories(
        ${MAESTRO_COMPONENT_VIT_DIR}/CortexM33-RW61X/Lib
        ${MAESTRO_COMPONENT_VIT_DIR}/CortexM33-RW61X/Lib/Inc
      )
    endif()
    #TBD: Other supported platforms
  endif() # CONFIG_MAESTRO_ENABLE_ELEMENT_VIT_SINK

  # Include Voice Seeker into the build
  # TBD: Based on specific platforms. Abstract the path
  if(CONFIG_MAESTRO_ENABLE_ELEMENT_VOICE_SEEKER)

    if(CONFIG_BOARD_RD_RW612_BGA AND CONFIG_BOARD_RD_RW612_BGA_RW612)
      #add_subdirectory_ifdef(CONFIG_MAESTRO_ENABLE_ELEMENT_VIT_SINK ${MAESTRO_COMPONENTS_DIR}/vit)
      set(MAESTRO_COMPONENT_VOICE_SEEKER_DIR ${MAESTRO_COMPONENTS_DIR}/voice_Seeker)
      set(MAESTRO_COMPONENT_VOICE_SEEKER_LIB_PATH ${MAESTRO_COMPONENT_VOICE_SEEKER_DIR}/ARM_CortexM33_noDSP/lib/libvoiceseeker_no_aec.a)

      zephyr_library_import(voice_seeker_lib ${MAESTRO_COMPONENT_VOICE_SEEKER_LIB_PATH})

      zephyr_include_directories(
        ${MAESTRO_COMPONENT_VOICE_SEEKER_DIR}/ARM_CortexM33_noDSP/include
        ${MAESTRO_COMPONENT_VOICE_SEEKER_DIR}/ARM_CortexM33_noDSP/rdsp_utilities_public/include
        ${MAESTRO_COMPONENT_VOICE_SEEKER_DIR}/ARM_CortexM33_noDSP/rdsp_utilities_public/rdsp_utilities_public
      )
      zephyr_library_sources(
        ${MAESTRO_COMPONENT_VOICE_SEEKER_DIR}/ARM_CortexM33_noDSP/rdsp_utilities_public/rdsp_utilities_public/memcheck.c
        ${MAESTRO_COMPONENT_VOICE_SEEKER_DIR}/ARM_CortexM33_noDSP/rdsp_utilities_public/rdsp_utilities_public/RdspMemoryUtilsPublic.c
      )
    endif()
    #TBD: Other supported platforms
  endif() # CONFIG_MAESTRO_ENABLE_ELEMENT_VOICE_SEEKER


  #add_subdirectory_ifdef(CONFIG_MAESTRO_ENABLE_ELEMENT_VIT_SINK ${MAESTRO_COMPONENTS_DIR}/voice_seeker)

  # Debugging messages
  # get_target_property(MY_SOURCES app SOURCES)
  # message("=> App Sources:" ${MY_SOURCES} "\n\n")
  # get_target_property(MY_DIRECTORIES app INCLUDE_DIRECTORIES)
  # message("=> Include Directories:" ${MY_DIRECTORIES} "\n\n")
else() # CONFIG_MAESTRO_AUDIO_FRAMEWORK=n

  # TODO: Delete me/comment me
  message("Maestro library excluded from the build. ")

endif()