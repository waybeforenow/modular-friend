/*
 * Processes incoming samples with Effects::LadderFilter::Filter and places them
 * into a thread-safe queue.
 *
 * This module should run MainLoop() in its own thread. Example:
 *
 * ```
 * #include <thread>
 *
 * auto input = new Friend::Input(queue, cutoff, resonance);
 * std::thread input_thread(std::bind(&Friend::Input::MainLoop, input));
 * ```
 *
 */

#ifndef __FRIEND_INPUT_H
#define __FRIEND_INPUT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include "alsa/asoundlib.h"
#include "friend-alsa-capture.h"
#include "friend-defaults.h"
#include "friend-flac-encoder.h"
#include "friend-safe-queue.h"
#include "ladder-filter/include/canonical.h"

namespace Friend {

class Input {
 private:
  SafeQueue<FRIEND__PCM_TYPE>* _output_queue;
  Effects::LadderFilter::Filter<float>* _left_filter;
  Effects::LadderFilter::Filter<float>* _right_filter;
  const snd_pcm_uframes_t _buffer_size;
  FRIEND__PCM_TYPE* _sample_buffer;
  ALSA::Capture* _capture;

 public:
  Input(SafeQueue<FRIEND__PCM_TYPE>*, float*, float*);
  ~Input();
  void MainLoop();
};

}  // namespace Friend

#endif  // __FRIEND_INPUT_H
