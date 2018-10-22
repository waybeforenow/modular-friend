#ifndef __FRIEND_OUTPUT_H
#define __FRIEND_OUTPUT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include "alsa/asoundlib.h"
#include "friend-alsa-playback.h"
#include "friend-flac-decoder.h"
#include "friend-safe-queue.h"

namespace Friend {

class Output {
 private:
  SafeQueue<float>* _output_queue;
  const snd_pcm_uframes_t _buffer_size;
  const ssize_t _sample_width;
  void* _output_buffer;
  ALSA::Playback* _playback;

 public:
  Output(SafeQueue<float>*);
  ~Output();
  void MainLoop();
};

}  // namespace Friend

#endif  // __FRIEND_OUTPUT_H
