#include "friend-alsa-playback.h"
#include <stdexcept>
#include "alsa/asoundlib.h"
#include "friend-defaults.h"

namespace Friend {
namespace ALSA {

Playback::Playback(FLAC__int32* left_buffer, FLAC__int32* right_buffer)
    : _device_name(FRIEND__ALSA_DEVICE_NAME),
      _sample_rate(FRIEND__SAMPLE_RATE),
      _pcm_format(FRIEND__PCM_FORMAT) {
  int err;
  _buffers = (void**)calloc(2, sizeof(void*));
  if (_buffers == nullptr) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  _buffers[0] = (void*)left_buffer;
  _buffers[1] = (void*)right_buffer;

  if ((err = snd_pcm_open(&_device_handle, _device_name,
                          SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  if ((err = snd_pcm_set_params(_device_handle, _pcm_format,
                                SND_PCM_ACCESS_RW_INTERLEAVED, 1, _sample_rate,
                                1, 500000)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }
}

Playback::~Playback() { snd_pcm_close(_device_handle); }

void Playback::PlaybackSamples(snd_pcm_uframes_t buffer_size) {
  snd_pcm_sframes_t frames =
      snd_pcm_writen(_device_handle, _buffers, buffer_size);
  if (frames < 0) {
    frames = snd_pcm_recover(_device_handle, frames, 0);
    if (frames < 0) {
      FRIEND__THROWEXCEPTION;  // XXX
    }
  }
}

}  // namespace ALSA
}  // namespace Friend
