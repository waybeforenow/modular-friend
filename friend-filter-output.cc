#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include "alsa/asoundlib.h"
#include "friend-defaults.h"
#include "friend-filter-output.h"
#include "friend-safe-queue.h"

namespace Friend {

Output::Output(SafeQueue<float>* queue)
    : _output_queue(queue),
      _buffer_size(32),
      _sample_width(snd_pcm_format_size(FRIEND__PCM_FORMAT, 1)),
      _output_buffer(::operator new(_buffer_size * _sample_width)),
      _playback(new ALSA::Playback((FLAC__int32*)_output_buffer, (FLAC__int32*)_output_buffer)) {}

Output::~Output() {
  delete _playback;
  ::operator delete(_output_buffer);
}

void Output::MainLoop() {
  snd_pcm_uframes_t b = 0;
  while (b < _buffer_size) {
    ((char*)_output_buffer)[_sample_width * b++] = _output_queue->dequeue();
  }

  _playback->PlaybackSamples(_buffer_size);
}

}  // namespace Friend
