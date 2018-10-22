#ifndef __FRIEND_ALSA_CAPTURE_H
#define __FRIEND_ALSA_CAPTURE_H

#include "alsa/asoundlib.h"
#include "friend-defaults.h"

namespace Friend {
namespace ALSA {

class Capture {
 private:
  snd_pcm_t* _device_handle;
  unsigned int _sample_rate;

 public:
  Capture();
  ~Capture();
  void CaptureSamples(FRIEND__PCM_TYPE* buffer, snd_pcm_uframes_t* buffer_size);
};

}  // namespace ALSA
}  // namespace Friend

#endif  // __FRIEND_ALSA_CAPTURE_H
