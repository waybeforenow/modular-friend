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

Input::Input(SafeQueue<float>* queue, uint16_t cutoff_frequency,
             float resonance)
    : _output_queue(queue),
      _filter(new Effects::LadderFilter::Filter(
          (float)cutoff_frequency, resonance, (float)FRIEND__SAMPLE_RATE)),
      _buffer_size(32) {
  // init ALSA capture device
  _capture = new ALSA::Capture();
}

Input::~Input() { delete _capture; }

void Input::MainLoop() {
  while (true) {
    snd_pcm_uframes_t _sample_buffer_size = _buffer_size;
    _capture->CaptureSamples(_sample_buffer, &_sample_buffer_size);
    for (const auto& sample : _sample_buffer)
      _queue->enqueue(_filter->tick(sample));
  }
}

}  // namespace Friend
