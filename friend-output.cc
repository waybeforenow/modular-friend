#include "friend-output.h"
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include "alsa/asoundlib.h"
#include "friend-defaults.h"

namespace Friend {

Output::Output(struct sockaddr_in address)
    : _address(address),
      _is_connected(false),
      _buffer_max(1024 * 32),
      _decode_buffer(new FLAC__byte[_buffer_max]),
      _decode_buffer_size(new unsigned int),
      _playback_left_buffer(new FLAC__int32[_buffer_max]),
      _playback_right_buffer(new FLAC__int32[_buffer_max]),
      _playback_buffer_size(new unsigned int) {
  // init alsa
  _playback = new ALSA::Playback(_playback_left_buffer, _playback_left_buffer);

  // init flac decoder
  _decoder_stream = new FLAC::Decoder(
      _decode_buffer, _decode_buffer_size, _playback_left_buffer,
      _playback_right_buffer, _playback_buffer_size, _buffer_max);
  ::FLAC__StreamDecoderInitStatus flac_err;
  if ((flac_err = _decoder_stream->init()) !=
      FLAC__STREAM_DECODER_INIT_STATUS_OK) {
    FRIEND__THROWEXCEPTIONWITHTEXT(
        FLAC__StreamDecoderInitStatusString[flac_err]);
  }

  // init network socket
  if ((_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  if (connect(_socket_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  _SetConnectedState(true);  // XXX wait for heartbeat from server
}

Output::~Output() {
  delete _playback;
  delete _decoder_stream;
  delete[] _decode_buffer;
  delete _decode_buffer_size;
  delete[] _playback_left_buffer;
  delete[] _playback_right_buffer;
  delete _playback_buffer_size;
}

void Output::_SetConnectedState(bool is_connected) {
  // XXX: set the output state of some LED
  _is_connected = is_connected;
}

void Output::MainLoop() {
  int bytes_to_read = 0;
  ioctl(_socket_fd, FIONREAD, &bytes_to_read);
  ssize_t err =
      read(_socket_fd, _decode_buffer,
           (_buffer_max < bytes_to_read) ? _buffer_max : bytes_to_read);
  if (err < 0) {
    FRIEND__THROWEXCEPTIONWITHTEXT(strerror(errno));
  }

  *_decode_buffer_size = err;

  _decoder_stream->process_until_end_of_stream();

  if (*_decode_buffer_size > 0) {  // XXX
    _playback->PlaybackSamples((snd_pcm_uframes_t)*_playback_buffer_size);
  }
}

}  // namespace Friend
