#ifndef _FRIEND_ALSA_CAPTURE_H
#define _FRIEND_ALSA_CAPTURE_H

#include "alsa/asoundlib.h"

namespace Friend {
namespace ALSA {

class Capture {
 private:
  const char* _device_name;
  snd_pcm_t* _device_handle;
  unsigned int _sample_rate;
  snd_pcm_format_t _pcm_format;

 public:
  Capture();
  ~Capture();
  void CaptureSamples(void* buffer, snd_pcm_uframes_t buffer_size);
};

}  // namespace ALSA
}  // namespace Friend

#endif  // _FRIEND_ALSA_CAPTURE_H
