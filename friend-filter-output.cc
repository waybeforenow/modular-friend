#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include "alsa/asoundlib.h"
#include "friend-defaults.h"
#include "friend-net-output.h"
#include "friend-safe-queue.h"

namespace Friend {

Output::Output(SafeQueue<float>* queue)
    : _output_queue(queue),
      _buffer_size(32),
      _output_buffer(::operator new(
          snd_pcm_format_size(FRIEND__PCM_FORMAT, _buffer_size))) {
  // init alsa
  _playback = new ALSA::Playback(_output_buffer, _output_buffer);
}

Output::~Output() { delete _playback; }

void Output::MainLoop() {
  snd_pcm_uframes_t b = 0;
  while (b < _buffer_size) {
    _output_buffer[b++] = _output_queue->dequeue();
  }

  _playback->PlaybackSamples(_buffer_size);
}

}  // namespace Friend
