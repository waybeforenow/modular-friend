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
  ALSA::Playback* _playback;
  SafeQueue<float>* _output_queue;
  const snd_pcm_uframes_t _buffer_size;
  void* _output_buffer;

 public:
  Output(SafeQueue*);
  ~Output();
  void MainLoop();
};

}  // namespace Friend

#endif  // __FRIEND_OUTPUT_H
