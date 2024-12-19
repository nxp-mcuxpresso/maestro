.. _Maestro_Sample_memtomem:

######################
Maestro mem2mem sample
######################

***********
Description
***********

Tests basic memory to memory pipeline.

**Function:**

#. Moves generated data with fixed size of 256B from memory source to memory sink.
#. Compares copied data to check if they're the same.
#. Returns ``Demo result: OK`` or ``Demo result: FAIL`` via UART.
