# Overview

`friend-filter-main` is a program that reads samples from an ALSA device,
transforms them through a digital implementation of a Moog 4-pole low-pass
ladder filter, and plays back the result of the transformation through the same
device.

# Building `friend-filter-main`

To build the default `friend-filter-main`, just run `make`.

## Defines

There are a few preprocessor defines in `friend-default.h` that control the
behavior of the program. You may be interested in overriding these.

* `FRIEND__SAMPLE_RATE`: The sample rate of the ALSA capture and playback.
  *Default:* `44100`
* `FRIEND__ALSA_DEVICE_NAME`: The name of the ALSA device to read to and write
  from. *Default:* `plughw:CARD=Audio,DEV=0`
* `FRIEND__PCM_FORMAT`: The sample format of the ALSA capture and playback,
  expressed as a [snd_pcm_format_t](https://www.alsa-project.org/alsa-doc/alsa-lib/group___p_c_m.html#gaa14b7f26877a812acbb39811364177f8) type. *Default:* `SND_PCM_FORMAT_FLOAT_LE`
* `FRIEND__PCM_TYPE`: The sample format of the ALSA capture and playback,
  expressed as a C++ type. *Default:* `float`
* `FRIEND__BUFFER_SIZE`: The buffer size to use for capture and playback.
  *Default:* `512`
