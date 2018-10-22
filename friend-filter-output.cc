#include "friend-filter-output.h"
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "alsa/asoundlib.h"
#include "friend-defaults.h"
#include "friend-safe-queue.h"

namespace Friend {

Output::Output(SafeQueue<FRIEND__PCM_TYPE>* queue)
    : _output_queue(queue),
      _buffer_size(32 * 2),
      _sample_width(snd_pcm_format_size(FRIEND__PCM_FORMAT, 1)),
      _output_buffer(new FRIEND__PCM_TYPE[_buffer_size]),
      _playback(new ALSA::Playback(_output_buffer)) {}

Output::~Output() {
  delete _playback;
  delete[] _output_buffer;
}

void Output::MainLoop() {
  while (true) {
    snd_pcm_uframes_t b = 0;
    while (b < _buffer_size) {
      ((char*)_output_buffer)[_sample_width * b++] =
          _output_queue->dequeue_left_channel();
      ((char*)_output_buffer)[_sample_width * b++] =
          _output_queue->dequeue_right_channel();
    }

    _playback->PlaybackSamples(_buffer_size);
  }
}

}  // namespace Friend
