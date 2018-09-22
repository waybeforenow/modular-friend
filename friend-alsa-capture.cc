#include "friend-alsa-capture.h"
#include <stdexcept>
#include <string>
#include "alsa/asoundlib.h"
#include "friend-defaults.h"

namespace Friend {
namespace ALSA {

Capture::Capture() : _sample_rate(FRIEND__SAMPLE_RATE) {
  int err;
  snd_pcm_hw_params_t* hw_params;

  if ((err = snd_pcm_open(&_device_handle, FRIEND__ALSA_DEVICE_NAME,
                          SND_PCM_STREAM_CAPTURE, 0)) < 0) {
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
                                          FRIEND__PCM_FORMAT)) < 0) {
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

void Capture::CaptureSamples(void* buffer, snd_pcm_uframes_t* buffer_size) {
  int err;

  if ((err = snd_pcm_readi(_device_handle, buffer, *buffer_size)) < 0) {
    if ((err = snd_pcm_recover(_device_handle, err, 1)) < 0) {
      std::string exception_text;
      exception_text.append("err from snd_pcm_readi: ");
      exception_text.append(snd_strerror(err));
      FRIEND__THROWEXCEPTIONWITHTEXT(exception_text.c_str());
    }
  }

  *buffer_size = err;
}

}  // namespace ALSA
}  // namespace Friend
