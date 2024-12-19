.. _Maestro_Sample_record_ram:

########################################################
Maestro sample for recording data from microphone to RAM
########################################################

***********
Description
***********

This sample records data from microphone (alias ``dmic0`` in devicetree) and stores them to a buffer in RAM.

Currently one PDM channel with fixed 16 kHz sample rate and 16 bit sample width is supported.

For configuration options, see Kconfig and prj.conf.

*****************
User Input/Output
*****************

   * Input:

      None.

   * Output:

      UART Output:

      * ``Demo result: OK`` if everything went OK
      * ``Demo result: FAIL`` otherwise

*******************
Supported platforms
*******************

Currently tested for:

   * RD_RW612_BGA.
