# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_PRJSEG_module.audio_voice.default_config)
    mcux_add_configuration(
        CC "-DSAI_XFER_QUEUE_SIZE=2"
    )

    mcux_add_iar_configuration(
        LD "--config_def=__stack_size__=0x1000 --config_def=__heap_size__=0x3000 --semihosting"
    )

    mcux_add_armgcc_configuration(
        LD "-Xlinker --defsym=__stack_size__=0x1000 -Xlinker --defsym=__heap_size__=0x1000"
    )
endif()

if (CONFIG_MCUX_PRJSEG_module.audio_voice.lpc_config)
    mcux_add_configuration(
        CC "-DPRINTF_ADVANCED_ENABLE=1 -DPRINTF_FLOAT_ENABLE=1"
    )

    mcux_add_iar_configuration(
        LD "--config_def=__stack_size__=0x1000 --config_def=__heap_size__=0x2500 --semihosting"
    )

    mcux_add_armgcc_configuration(
        LD "-Xlinker --defsym=__stack_size__=0x1000 -Xlinker --defsym=__heap_size__=0x2000"
    )
endif()


if (CONFIG_MCUX_PRJSEG_MAESTRO_EXAMPLE_VIT_MODEL_EN_ENABLE)
    mcux_add_macro(
        CC "-DVIT_MODEL_EN"
        TOOLCHAINS armgcc mcux
    )
endif()
if (CONFIG_MCUX_PRJSEG_MAESTRO_EXAMPLE_VIT_MODEL_CN_ENABLE)
    mcux_add_macro(
        CC "-DVIT_MODEL_CN"
        TOOLCHAINS armgcc mcux
    )
endif()
if (CONFIG_MCUX_PRJSEG_MAESTRO_EXAMPLE_VIT_MODEL_DE_ENABLE)
    mcux_add_macro(
        CC "-DVIT_MODEL_DE"
        TOOLCHAINS armgcc mcux
    )
endif()
if (CONFIG_MCUX_PRJSEG_MAESTRO_EXAMPLE_VIT_MODEL_ES_ENABLE)
    mcux_add_macro(
        CC "-DVIT_MODEL_ES"
        TOOLCHAINS armgcc mcux
    )
endif()
if (CONFIG_MCUX_PRJSEG_MAESTRO_EXAMPLE_VIT_MODEL_FR_ENABLE)
    mcux_add_macro(
        CC "-DVIT_MODEL_FR"
        TOOLCHAINS armgcc mcux
    )
endif()
if (CONFIG_MCUX_PRJSEG_MAESTRO_EXAMPLE_VIT_MODEL_IT_ENABLE)
    mcux_add_macro(
        CC "-DVIT_MODEL_IT"
        TOOLCHAINS armgcc mcux
    )
endif()
if (CONFIG_MCUX_PRJSEG_MAESTRO_EXAMPLE_VIT_MODEL_JA_ENABLE)
    mcux_add_macro(
        CC "-DVIT_MODEL_JA"
        TOOLCHAINS armgcc mcux
    )
endif()
if (CONFIG_MCUX_PRJSEG_MAESTRO_EXAMPLE_VIT_MODEL_KO_ENABLE)
    mcux_add_macro(
        CC "-DVIT_MODEL_KO"
        TOOLCHAINS armgcc mcux
    )
endif()
if (CONFIG_MCUX_PRJSEG_MAESTRO_EXAMPLE_VIT_MODEL_PT_ENABLE)
    mcux_add_macro(
        CC "-DVIT_MODEL_PT"
        TOOLCHAINS armgcc mcux
    )
endif()
if (CONFIG_MCUX_PRJSEG_MAESTRO_EXAMPLE_VIT_MODEL_TR_ENABLE)
    mcux_add_macro(
        CC "-DVIT_MODEL_TR"
        TOOLCHAINS armgcc mcux
    )
endif()