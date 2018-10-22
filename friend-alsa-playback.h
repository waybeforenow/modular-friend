#ifndef __FRIEND_ALSA_PLAYBACK_H
#define __FRIEND_ALSA_PLAYBACK_H

#include "FLAC++/decoder.h"
#include "alsa/asoundlib.h"

namespace Friend {
namespace ALSA {

class Playback {
 private:
  const char* _device_name;
  snd_pcm_t* _device_handle;
  unsigned int _sample_rate;
  snd_pcm_format_t _pcm_format;
  void** _buffers;

 public:
  Playback(FLAC__int32* left_buffer, FLAC__int32* right_buffer);
  ~Playback();
  void PlaybackSamples(snd_pcm_uframes_t buffer_size);
};

}  // namespace ALSA
}  // namespace Friend

#endif  // __FRIEND_ALSA_PLAYBACK_H
