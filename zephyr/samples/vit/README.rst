.. _vit:

Maestro voice detection sample using VIT
#####################################################

Description
***********
Records data from microphone (alias **dmic0** in devicetree) and detects voice commands in selected language selected by MAESTRO_EXAMPLE_VIT_LANGUAGE.

Detected commands are printed via UART.

Language model may be changed via Kconfig using MAESTRO_EXAMPLE_VIT_LANGUAGE selection.

References
**********

- VIT library: https://www.nxp.com/design/design-center/software/embedded-software/voice-intelligent-technology-wake-word-and-voice-command-engines:VOICE-INTELLIGENT-TECHNOLOGY

Building and Running
********************

This project requires NXP board supported by VIT library.

Currently tested for:

- RD_RW612_BGA.