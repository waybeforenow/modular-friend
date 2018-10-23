#ifndef __FRIEND_OUTPUT_H
#define __FRIEND_OUTPUT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include "alsa/asoundlib.h"
#include "friend-alsa-playback.h"
#include "friend-defaults.h"
#include "friend-flac-decoder.h"
#include "friend-safe-queue.h"

namespace Friend {

class Output {
 private:
  SafeQueue<FRIEND__PCM_TYPE>* _output_queue;
  const snd_pcm_uframes_t _buffer_size;
  FRIEND__PCM_TYPE* _output_buffer;
  ALSA::Playback* _playback;

 public:
  Output(SafeQueue<FRIEND__PCM_TYPE>*);
  ~Output();
  void MainLoop();
};

}  // namespace Friend

#endif  // __FRIEND_OUTPUT_H
