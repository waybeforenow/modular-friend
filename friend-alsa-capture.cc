#include "friend-alsa-capture.h"
#include <stdexcept>
#include "alsa/asoundlib.h"

namespace Friend {
namespace ALSA {

Capture::Capture()
    : _device_name("XXX"),
      _sample_rate(44100),
      _pcm_format(SND_PCM_FORMAT_U16_LE) {
  int err;
  snd_pcm_hw_params_t* hw_params;

  if ((err = snd_pcm_open(&_device_handle, _device_name, SND_PCM_STREAM_CAPTURE,
                          0)) < 0) {
    throw std::exception();  // XXX
  }

  if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
    throw std::exception();  // XXX
  }

  if ((err = snd_pcm_hw_params_any(_device_handle, hw_params)) < 0) {
    throw std::exception();  // XXX
  }

  if ((err = snd_pcm_hw_params_set_access(_device_handle, hw_params,
                                          SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    throw std::exception();  // XXX
  }

  if ((err = snd_pcm_hw_params_set_format(_device_handle, hw_params,
                                          _pcm_format)) < 0) {
    throw std::exception();  // XXX
  }

  if ((err = snd_pcm_hw_params_set_rate_near(_device_handle, hw_params,
                                             &_sample_rate, 0)) < 0) {
    throw std::exception();  // XXX
  }

  if ((err = snd_pcm_hw_params_set_channels(_device_handle, hw_params, 2)) <
      0) {
    throw std::exception();  // XXX
  }

  if ((err = snd_pcm_hw_params(_device_handle, hw_params)) < 0) {
    throw std::exception();  // XXX
  }

  snd_pcm_hw_params_free(hw_params);

  if ((err = snd_pcm_prepare(_device_handle)) < 0) {
    throw std::exception();  // XXX
  }
}

Capture::~Capture() { snd_pcm_close(_device_handle); }

void Capture::CaptureSamples(void* buffer, snd_pcm_uframes_t buffer_size) {
  int err;

  if ((err = snd_pcm_readi(_device_handle, buffer, buffer_size)) !=
      buffer_size) {
    throw std::exception();  // XXX
  }
}

}  // namespace ALSA
}  // namespace Friend