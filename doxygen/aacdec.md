# AAC decoder {#aacdec_start}

## AAC decoder features
- The AAC decoder implementation supports the following:
+ Supported profile : AAC-LC
+ Sampling rate : 8 kHz, 11.025 kHz, 12 kHz, 16 kHz, 22.50 kHz, 24 kHz, 32 kHz, 44.1 kHz, 48 kHz
+ Channel :	stereo and mono
+ Bits per samples : 16 bit
+ Container format : (MPEG-2 Style)AAC transport format - ADTS and ADIF.

## Specification and reference

## Performance

### Memory information
The memory usage of the decoder in bytes is:
* Code/flash    =  26332 + 19264 = 45596
* Data/RAM      =  26832

| Section | Size |
|--------:|-----:|
| .text | 26332  |
| .ro & .const | 19264 |
| .bss | 26832 |

### CPU usage
* CPU core clock in MHz: 20.97.

| Track type | Duration of track in second | Frame size in bytes | Performance MIPS of codec (in MHz)  |
|:----------:|:---------------------------:|:-------------------:|:----------------------------------:|
|48 kHz, stereo | 38 s  | 4096 | 12.2 MHz |
