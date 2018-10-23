#include "friend-alsa-playback.h"
#include <stdexcept>
#include "alsa/asoundlib.h"
#include "friend-defaults.h"

namespace Friend {
namespace ALSA {

Playback::Playback(FRIEND__PCM_TYPE* playback_buffer)
    : _device_name(FRIEND__ALSA_DEVICE_NAME),
      _sample_rate(FRIEND__SAMPLE_RATE),
      _pcm_format(FRIEND__PCM_FORMAT),
      _playback_buffer(playback_buffer) {
  int err;

  if ((err = snd_pcm_open(&_device_handle, _device_name,
                          SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  if ((err = snd_pcm_set_params(_device_handle, _pcm_format,
                                SND_PCM_ACCESS_RW_INTERLEAVED, 2, _sample_rate,
                                0, 500000)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }
}

Playback::~Playback() { snd_pcm_close(_device_handle); }

void Playback::PlaybackSamples(snd_pcm_uframes_t buffer_size) {
  snd_pcm_sframes_t frames =
      snd_pcm_writei(_device_handle, (void*)_playback_buffer, buffer_size);
  if (frames < 0) {
    if (frames == -EAGAIN) {
      return;
    }
    frames = snd_pcm_recover(_device_handle, frames, 1);
    if (frames < 0) {
      FRIEND__THROWEXCEPTIONWITHTEXT(snd_strerror(frames));
    }
  }
}

}  // namespace ALSA
}  // namespace Friend
