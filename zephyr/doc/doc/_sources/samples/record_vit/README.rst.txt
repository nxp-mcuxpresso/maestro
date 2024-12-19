.. _Maestro_sample_record_vit:

########################################
Maestro voice detection sample using VIT
########################################

***********
Description
***********

Records data from microphone (alias ``dmic0`` in devicetree) and detects voice commands from selected language model. Detected commands are printed via UART.

Language model may be changed via Kconfig using ``CONFIG_MAESTRO_EXAMPLE_VIT_LANGUAGE`` selection. For other configuration options, see example's ``Kconfig`` and ``prj.conf``.

This project requires an NXP board supported by the VIT library.

The example has to be modified if a new board needs to be added. Please create an issue in that case.

*****************
User Input/Output
*****************

   * Input:

      None.

   * Output:

      UART Output:

         * List of voice commands the model can detect (printed immediately after start)
         * ``<Specific voice command>`` if voice command was detected
         * ``Demo result: FAIL`` otherwise

************
Dependencies
************

   * VIT library: https://www.nxp.com/design/design-center/software/embedded-software/voice-intelligent-technology-wake-word-and-voice-command-engines:VOICE-INTELLIGENT-TECHNOLOGY

*******************
Supported platforms
*******************

Currently tested for:

   * RD_RW612_BGA.
