#include "friend-filter-input.h"
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include "alsa/asoundlib.h"
#include "friend-defaults.h"
#include "friend-safe-queue.h"
#include "ladder-filter/include/canonical.h"

namespace Friend {

Input::Input(SafeQueue<FRIEND__PCM_TYPE>* queue, float* cutoff_frequency,
             float* resonance)
    : _output_queue(queue),
      _left_filter(new Effects::LadderFilter::Filter<FRIEND__PCM_TYPE>(
          cutoff_frequency, resonance, (float)FRIEND__SAMPLE_RATE)),
      _right_filter(new Effects::LadderFilter::Filter<FRIEND__PCM_TYPE>(
          cutoff_frequency, resonance, (float)FRIEND__SAMPLE_RATE)),
      _buffer_size(FRIEND__BUFFER_SIZE),
      _sample_buffer(new FRIEND__PCM_TYPE[_buffer_size * 2]),
      _capture(new ALSA::Capture()) {}

Input::~Input() {
  delete _capture;
  delete _left_filter;
  delete _right_filter;
  delete[] _sample_buffer;
}

void Input::MainLoop() {
  while (true) {
    snd_pcm_uframes_t _sample_buffer_size = _buffer_size;
    _capture->CaptureSamples(_sample_buffer, &_sample_buffer_size);

    snd_pcm_uframes_t i = 0;
    while (i < _buffer_size * 2) {
      _output_queue->enqueue_left_channel(
          _left_filter->tick(_sample_buffer[i++]));
      _output_queue->enqueue_right_channel(
          _right_filter->tick(_sample_buffer[i++]));
    }
  }
}

}  // namespace Friend
