.. _Maestro_Sample_decoder:

######################
Maestro decoder sample
######################

***********
Description
***********

Tests and demonstrates decoder functionality in Maestro pipeline.

Supported decoders:

    * MP3
    * WAV
    * AAC
    * FLAC
    * OPUS with OGG envelop
    * (RAW OPUS - TBD)

Data Input:

    * Prepared encoded audio data (part of Maestro repository, folder ``zephyr/audioTracks``)
    * Prepared decoded audio data (RAW PCM format, part of Maestro repository, folder ``zephyr/audioTracks``)

Function:

    #. Loads encoded data into source buffer stored in RAM
    #. Decodes audio data using selected decoder and stores data in RAM
    #. Compares prepared data with decoded data to check if its the same
    #. Prints ``Demo result: OK`` or ``Demo result: FAIL`` via UART

*****************
User Input/Output
*****************

    * Input:

        None

    * Output:

        UART Output

            * ``Demo result: OK`` if everything went OK
            * ``Demo result: FAIL`` otherwise

************
Dependencies
************

    * Audio voice component library (pulled in by Maestro's west), containing Decoder libraries

*************
Configuration
*************

    * See ``prj.conf`` for user input sections

        * Selecting decoder may be done by enabling ``CONFIG_MAESTRO_EXAMPLE_DECODER_SELECTED_<DECODER_NAME>`` in ``prj.conf`` file. When no decoder is selected, default one (WAV) is used instead.

        * System settings should be modified (stack size, heap size) based on selected decoder and system capabilities/requirements in ``prj.conf``.

    * For other configuration options, see example's ``Kconfig`` and ``prj.conf``.

*******************
Supported platforms
*******************

Currently tested for:

    * RD_RW612_BGA - Working decoders: FLAC, WAV, OPUS OGG