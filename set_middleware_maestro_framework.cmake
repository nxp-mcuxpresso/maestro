include_guard(GLOBAL)


if (CONFIG_USE_middleware_maestro_framework_opus)
# Add set(CONFIG_USE_middleware_maestro_framework_opus true) in config.cmake to use this component

message("middleware_maestro_framework_opus component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/bands.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/celt.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/celt_decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/celt_encoder.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/celt_lpc.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/cwrs.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/entcode.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/entdec.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/entenc.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/kiss_fft.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/laplace.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/mathops.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/mdct.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/modes.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/pitch.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/quant_bands.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/rate.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt/vq.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/A2NLSF.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/CNG.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/HP_variable_cutoff.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/LPC_analysis_filter.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/LPC_fit.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/LPC_inv_pred_gain.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/LP_variable_cutoff.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/NLSF2A.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/NLSF_VQ.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/NLSF_VQ_weights_laroia.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/NLSF_decode.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/NLSF_del_dec_quant.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/NLSF_encode.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/NLSF_stabilize.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/NLSF_unpack.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/NSQ.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/NSQ_del_dec.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/PLC.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/VAD.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/VQ_WMat_EC.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/ana_filt_bank_1.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/biquad_alt.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/bwexpander.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/bwexpander_32.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/check_control_input.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/code_signs.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/control_SNR.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/control_audio_bandwidth.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/control_codec.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/debug.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/dec_API.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/decode_core.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/decode_frame.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/decode_indices.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/decode_parameters.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/decode_pitch.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/decode_pulses.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/decoder_set_fs.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/enc_API.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/encode_indices.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/encode_pulses.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/gain_quant.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/init_decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/init_encoder.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/inner_prod_aligned.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/interpolate.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/lin2log.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/log2lin.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/pitch_est_tables.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/process_NLSFs.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/quant_LTP_gains.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/resampler.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/resampler_down2.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/resampler_down2_3.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/resampler_private_AR2.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/resampler_private_IIR_FIR.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/resampler_private_down_FIR.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/resampler_private_up2_HQ.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/resampler_rom.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/shell_coder.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/sigm_Q15.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/sort.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/stereo_LR_to_MS.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/stereo_MS_to_LR.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/stereo_decode_pred.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/stereo_encode_pred.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/stereo_find_predictor.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/stereo_quant_pred.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/sum_sqr_shift.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/table_LSF_cos.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/tables_LTP.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/tables_NLSF_CB_NB_MB.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/tables_NLSF_CB_WB.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/tables_gain.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/tables_other.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/tables_pitch_lag.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/tables_pulses_per_block.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/LTP_analysis_filter_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/LTP_scale_ctrl_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/apply_sine_window_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/autocorr_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/burg_modified_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/corrMatrix_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/encode_frame_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/find_LPC_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/find_LTP_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/find_pitch_lags_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/find_pred_coefs_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/k2a_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/k2a_Q16_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/noise_shape_analysis_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/pitch_analysis_core_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/process_gains_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/regularize_correlations_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/residual_energy16_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/residual_energy_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/schur64_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/schur_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/vector_ops_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed/warped_autocorrelation_FIX.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/src/opus.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/src/opus_cci.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/src/opus_cci_raw.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/src/opus_decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/src/opus_encoder.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/src/opus_multistream.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/src/opus_multistream_decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/src/opus_multistream_encoder.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/src/repacketizer.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/celt
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/include
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/silk/fixed
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/src
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DHAVE_CONFIG_H
  )

endif()


endif()


if (CONFIG_USE_middleware_maestro_framework_opusfile)
# Add set(CONFIG_USE_middleware_maestro_framework_opusfile true) in config.cmake to use this component

message("middleware_maestro_framework_opusfile component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opusfile/src/info.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opusfile/src/internal.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opusfile/src/opusfile.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opusfile/src/stream.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opusfile/include
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opusfile/src
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DHAVE_CONFIG_H
  )

endif()


endif()


if (CONFIG_USE_middleware_maestro_framework_ogg)
# Add set(CONFIG_USE_middleware_maestro_framework_ogg true) in config.cmake to use this component

message("middleware_maestro_framework_ogg component is included from ${CMAKE_CURRENT_LIST_FILE}.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/ogg/src/framing.c
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/ogg/src/memory.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/ogg/include
  ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/ogg/src
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DHAVE_CONFIG_H
  )

endif()


endif()


if (CONFIG_USE_middleware_maestro_framework_doc)
# Add set(CONFIG_USE_middleware_maestro_framework_doc true) in config.cmake to use this component

message("middleware_maestro_framework_doc component is included from ${CMAKE_CURRENT_LIST_FILE}.")


endif()


if (CONFIG_USE_middleware_maestro_framework_codecs)
# Add set(CONFIG_USE_middleware_maestro_framework_codecs true) in config.cmake to use this component

message("middleware_maestro_framework_codecs component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_maestro_framework)

if(CONFIG_CORE STREQUAL cm7f AND (CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/./libs/cm7f/armgcc/release/libmp3.a
      ${CMAKE_CURRENT_LIST_DIR}/./libs/cm7f/armgcc/release/libwav.a
      ${CMAKE_CURRENT_LIST_DIR}/./libs/cm7f/armgcc/release/libaac.a
      ${CMAKE_CURRENT_LIST_DIR}/./libs/cm7f/armgcc/release/libflac.a
      ${CMAKE_CURRENT_LIST_DIR}/./libs/cm7f/armgcc/release/libssrc.a
      -Wl,--end-group
  )
endif()

if(CONFIG_CORE STREQUAL cm33 AND (CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/./libs/cm33f/armgcc/release/libmp3.a
      ${CMAKE_CURRENT_LIST_DIR}/./libs/cm33f/armgcc/release/libwav.a
      ${CMAKE_CURRENT_LIST_DIR}/./libs/cm33f/armgcc/release/libaac.a
      ${CMAKE_CURRENT_LIST_DIR}/./libs/cm33f/armgcc/release/libflac.a
      ${CMAKE_CURRENT_LIST_DIR}/./libs/cm33f/armgcc/release/libssrc.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_maestro_framework_codecs dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()


if (CONFIG_USE_middleware_maestro_framework)
# Add set(CONFIG_USE_middleware_maestro_framework true) in config.cmake to use this component

message("middleware_maestro_framework component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if(CONFIG_USE_middleware_maestro_framework_doc)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/core/pad.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/core/pipeline.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/core/streamer.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/core/streamer_element.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/core/streamer_msg.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/decoders/cci_dec/ccidec.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/decoders/cci_dec/codec_interface.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/decoders/cci_dec/codecextractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/devices/audio_sink_pcmrtos.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/devices/audio_src_pcmrtos.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/audio_proc.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/audio_sink.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/audio_src.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/decoder_pads.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/file_sink.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/file_src_freertos.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/mem_sink.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/vit_sink.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/mem_src.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/netbuf_src.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/parsers/cci/cci_codec_type_conversion.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/pipelines/streamer_audiosrc.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/pipelines/streamer_fs.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/pipelines/streamer_mic2file.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/pipelines/streamer_opusmem2mem.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/pipelines/streamer_pcm_speaker.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/pipelines/streamer_pcm_speaker_mem.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/pipelines/streamer_testeapfile2file.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/pipelines/streamer_vit.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/pipelines/streamer_vit_filesink.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements/encoder.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/encoders/opus/opusenc_cei.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/cci/metadata/src/mp3/mp3_extractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/cci/metadata/src/mp3/mp3_id3v2.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/cci/metadata/src/mp3/mp3_vbr.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/cci/metadata/src/adpcm/adpcm_extractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/cci/metadata/src/adpcm/wav_common.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/cci/metadata/src/opus/opus_extractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/cci/metadata/src/aac/aac_extractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/cci/metadata/src/flac/flac_extractmetadata.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/utils/file_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/utils/general_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/utils/maestro_logging.c
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/utils/ringbuffer.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/./config
  ${CMAKE_CURRENT_LIST_DIR}/./inc
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/cci/include
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/core
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/decoders/cci_dec
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/devices
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/elements
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/encoders/opus
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/encoders/cei
  ${CMAKE_CURRENT_LIST_DIR}/./streamer/pipelines
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCASCFG_PLATFORM_FREERTOS
    -DFSL_OS_SELECTED=SDK_OS_FREERTOS
    -DFSL_OSA_TASK_ENABLE=1
  )

  if(CONFIG_TOOLCHAIN STREQUAL iar)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      --vla
      --align_sp_on_irq
      --macro_positions_in_diagnostics
      --header_context
      --no_unroll
      --no_inline
      --no_tbaa
      --no_unaligned_access
      --dlib_config full
      --diag_suppress Pa050,Pa082,Pe186,Pe815
    )
  endif()

endif()

else()

message(SEND_ERROR "middleware_maestro_framework dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()

