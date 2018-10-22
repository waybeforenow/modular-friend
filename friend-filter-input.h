#ifndef __FRIEND_INPUT_H
#define __FRIEND_INPUT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include "alsa/asoundlib.h"
#include "friend-alsa-capture.h"
#include "friend-flac-encoder.h"
#include "friend-safe-queue.h"
#include "ladder-filter/include/canonical.h"

namespace Friend {

class Input {
 private:
  ALSA::Capture* _capture;
  SafeQueue<float>* _output_queue;
  Effects::LadderFilter::Filter<float>* _filter;
  const snd_pcm_uframes_t _buffer_size;
  const ssize_t _sample_width;
  void* _sample_buffer;

 public:
  Input(SafeQueue<float>*, uint16_t, float);
  ~Input();
  void MainLoop();
};

}  // namespace Friend

#endif  // __FRIEND_INPUT_H
