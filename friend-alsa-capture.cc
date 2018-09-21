#include "friend-alsa-capture.h"
#include <stdexcept>
#include "alsa/asoundlib.h"
#include "friend-defaults.h"

namespace Friend {
namespace ALSA {

Capture::Capture()
    : _device_name(FRIEND__ALSA_DEVICE_NAME),
      _sample_rate(FRIEND__SAMPLE_RATE),
      _pcm_format(FRIEND__PCM_FORMAT) {
  int err;
  snd_pcm_hw_params_t* hw_params;

  if ((err = snd_pcm_open(&_device_handle, _device_name, SND_PCM_STREAM_CAPTURE,
                          0)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  if ((err = snd_pcm_hw_params_any(_device_handle, hw_params)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  if ((err = snd_pcm_hw_params_set_access(_device_handle, hw_params,
                                          SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  if ((err = snd_pcm_hw_params_set_format(_device_handle, hw_params,
                                          _pcm_format)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  if ((err = snd_pcm_hw_params_set_rate_near(_device_handle, hw_params,
                                             &_sample_rate, 0)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  if ((err = snd_pcm_hw_params_set_channels(_device_handle, hw_params, 2)) <
      0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  if ((err = snd_pcm_hw_params(_device_handle, hw_params)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  snd_pcm_hw_params_free(hw_params);

  if ((err = snd_pcm_prepare(_device_handle)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }
}

Capture::~Capture() { snd_pcm_close(_device_handle); }

void Capture::CaptureSamples(void* buffer, snd_pcm_uframes_t buffer_size) {
  int err;

  if ((err = snd_pcm_readi(_device_handle, buffer, buffer_size)) !=
      buffer_size) {
    FRIEND__THROWEXCEPTION;  // XXX
  }
}

}  // namespace ALSA
}  // namespace Friend
