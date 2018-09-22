#ifndef _FRIEND_ALSA_CAPTURE_H
#define _FRIEND_ALSA_CAPTURE_H

#include "alsa/asoundlib.h"

namespace Friend {
namespace ALSA {

class Capture {
 private:
  snd_pcm_t* _device_handle;
  unsigned int _sample_rate;

 public:
  Capture();
  ~Capture();
  void CaptureSamples(void* buffer, snd_pcm_uframes_t* buffer_size);
};

}  // namespace ALSA
}  // namespace Friend

#endif  // _FRIEND_ALSA_CAPTURE_H
