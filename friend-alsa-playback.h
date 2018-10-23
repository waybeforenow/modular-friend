/*
 * Wrapper class for snd_pcm_writei(). Initializes and configures the necessary
 * ALSA data structures upon instantiation.
 *
 */

#ifndef __FRIEND_ALSA_PLAYBACK_H
#define __FRIEND_ALSA_PLAYBACK_H

#include "alsa/asoundlib.h"
#include "friend-defaults.h"

namespace Friend {
namespace ALSA {

class Playback {
 private:
  const char* _device_name;
  snd_pcm_t* _device_handle;
  unsigned int _sample_rate;
  snd_pcm_format_t _pcm_format;
  FRIEND__PCM_TYPE* _playback_buffer;

 public:
  Playback(FRIEND__PCM_TYPE*);
  ~Playback();
  void PlaybackSamples(snd_pcm_uframes_t buffer_size);
};

}  // namespace ALSA
}  // namespace Friend

#endif  // __FRIEND_ALSA_PLAYBACK_H
