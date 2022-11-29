# FLAC decoder {#flacdec_start}

## FLAC decoder features
- The FLAC decoder implementation support the following:
+ Sampling rate: 8 kHz, 11.05 kHz, 12 kHz, 16 kHz, 22.50 kHz, 32 kHz, 44.1 kHz, and 48 kHz.
+ Channel :	stereo and mono
+ Bits per samples : 16 bits

## Specification and reference

### Official website
* FLAC lossless audio codec is at https://xiph.org/flac.

### Inbound licensing
* For licensing information please refer to FLAC's official website: https://xiph.org/flac/license.html.

## Performance

### Memory information
The memory usage of the decoder in bytes is:
* Code/flash    = 15744 + 2080 = 17824
* Data/RAM      = 27936

| Section | Size |
|--------:|-----:|
| .text |  15744 |
| .ro & .const | 2080 |
| .bss | 27936 |


### CPU usage
* Output frame size: 16384 bytes.
* CPU core clock in MHz: 20.97.

| Track type | Duration of track in second |  Performance MIPS of codec (in MHz) |
|:----------:|:---------------------------:|------------------------------------:|
|48 kHz, stereo| 76 s | 30.7 MHz  |
|32 kHz, stereo| 76 s | 20.3 MHz  |
|8 kHz, stereo| 37 s | 5.34 MHz |

### Following test cases are performed:
* Audio format listening test
* Audio quality test

For all above test cases, test tracks are played through the end without any distortion, glitching, hanging, or crashing.



