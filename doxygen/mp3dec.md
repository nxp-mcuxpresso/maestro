# MP3 decoder {#mp3dec_start}

## MP3 decoder features
* MP3 decoder supports mpeg-1, mpeg-2, mpeg-2.5.
* All MP3 features supported , including joint stereo, mid-side stereo, intensity stereo, and dual channel.
* Supported sampling rate: 8 kHz, 11.025 kHz, 12 kHz, 16 kHz, 22.50 kHz, 24 kHz, 32 kHz, 44.1 kHz and 48 kHz.
* Supported channel: stereo and mono
* Supported bits per samples: 16 bit
* Supported bit rate: 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 320, 384, 416, and 448.

## Performance

### Memory information
The memory usage of the decoder (data obtained from IAR compiler) in bytes is:
* Code/flash = 26884 + 18372 = 45256
* RAM = 16200

| Section | Size |
|--------:|-----:|
| .text | 26884 |
| .ro & .const | 18372 |
| .bss | 16200 |

### CPU usage
The performance of the decoder was measured using the real hardware platform (RT1060).
* CPU core clock in MHz: 600.

| Track type | Duration of track in second | Frame size in bytes | Performance MIPS of codec (in MHz)  |
|:----------:|:---------------------------:|:-------------------:|:----------------------------------:|
|320 Kbps, 44.1 kHz, stereo| 358 s  | 2304 | ~24 MHz |
|192 Kbps, 48 kHz, stereo| 10 s  | 2304 | ~18 MHz |

