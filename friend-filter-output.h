/*
 * Places incoming samples from a thread-safe queue into a buffer and plays them
 * from the soundcard.
 *
 * This module should run MainLoop() in its own thread. Example:
 *
 * ```
 * #include <thread>
 *
 * auto input = new Friend::Output(queue);
 * std::thread output_thread(std::bind(&Friend::Output::MainLoop, output));
 * ```
 *
 */

#ifndef __FRIEND_INPUT_H
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
