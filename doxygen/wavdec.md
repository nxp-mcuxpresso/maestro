# WAV decoder

## WAV decoder features
- The WAV decoder implementation support the following:
+ Sampling rate: 8 kHz, 11.025kHz, 16 kHz, 22.50 kHz, 32 kHz, 44.1 kHz, and 48 kHz.
+ Channel:	stereo and mono
+ PCM format with 8/16/24 bits per sample.

## Performance

### Memory information
The memory usage of the decoder in bytes is:
* Code/flash    = 6260 + 342 = 6602
* Data/RAM      =  16 + 20696 = 20712

| Section | Size |
|--------:|-----:|
| .text |  6260 |
| .ro & .const | 342 |
| .bss | 20696 |
| .data | 16 |


### CPU usage
The performance of the decoder was measured using the decoder standalone unit test.
* CPU core clock in MHz: 20.97 MHz.

| Track type | Duration of track in second | Frame size in bytes | Performance MIPS of codec (in MHz) |
|:----------:|:---------------------------:|:-------------------:|:----------------------------------:|
|48 kHz, stereo, PCM|  12 s | 4096 | 9.68 MHz  |

### Following test cases were performed:
* Audio format listening test
* Audio quality test

For all above test cases, test tracks are played through the end without any distortion, glitching, hanging, or crashing.



