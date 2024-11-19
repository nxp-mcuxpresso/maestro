.. _Maestro_Sample_encoder:

######################
Maestro encoder sample
######################

***********
Description
***********

Tests and demonstrates encoder functionality in Maestro pipeline.

Supported encoders:
    * OPUS with OGG envelop - TBD
    * RAW OPUS - TBD

Input:

    * Prepared decoded audio data (RAW PCM format, part of Maestro repository)
    * Prepared encoded audio data (part of Maestro repository)

Function:

#. Loads RAW data into source buffer stored in RAM
#. Encodes audio data using selected encoder and stores data in RAM
#. Compares prepared data with decoded data if same
#. Prints ``Demo result: OK`` or ``Demo result: FAIL`` via UART

************
Dependencies
************

- Audio voice component library (pulled in by Maestro's west), containing Encoder libraries

*****************
User Input/Output
*****************

Input:

- None

Output:

- UART Output

   - ``Demo result: OK`` if everything went OK
   - ``Demo result: FAIL`` otherwise

*************
Configuration
*************

    * ``See prj.conf`` for user input sections

        * Selecting encoder may be done by enabling ``CONFIG_MAESTRO_EXAMPLE_ENCODER_SELECTED_<ENCODER_NAME>`` in ``prj.conf`` file. When no encoder is selected, default one (OPUS) is used instead.

        * System settings should be modified (stack size, heap size) based on selected encoder and system capabilities/requirements in ``prj.conf`` file.

    * For other configuration options, see example's ``Kconfig`` and ``prj.conf``.

*******************
Supported platforms
*******************

Currently tested for:

    * RD_RW612_BGA - Working encoders: None.
  