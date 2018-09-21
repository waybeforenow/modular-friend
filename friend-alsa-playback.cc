#include "friend-alsa-playback.h"
#include <stdexcept>
#include "alsa/asoundlib.h"

namespace Friend {
namespace ALSA {

Playback::Playback()
    : _device_name("XXX"),
      _sample_rate(44100),
      _pcm_format(SND_PCM_FORMAT_U16_LE) {
  int err;
  if ((err = snd_pcm_open(&_device_handle, _device_name,
                          SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    throw std::exception();  // XXX
  }

  if ((err = snd_pcm_set_params(_device_handle, _pcm_format,
                                SND_PCM_ACCESS_RW_INTERLEAVED, 1, _sample_rate,
                                1, 500000)) < 0) {
    throw std::exception();  // XXX
  }
}

Playback::~Playback() { snd_pcm_close(_device_handle); }

void Playback::PlaybackSamples(void* buffer, snd_pcm_uframes_t buffer_size) {
  snd_pcm_sframes_t frames =
      snd_pcm_writei(_device_handle, buffer, buffer_size);
  if (frames < 0) {
    frames = snd_pcm_recover(_device_handle, frames, 0);
    if (frames < 0) {
      throw std::exception();  // XXX
    }
  }
}

}  // namespace ALSA
}  // namespace Friend
