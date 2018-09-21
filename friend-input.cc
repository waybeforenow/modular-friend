#include "friend-input.h"
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include "alsa/asoundlib.h"
#include "friend-defaults.h"

#ifdef DEBUG
#include <iostream>
#endif  // DEBUG

namespace Friend {

Input::Input(uint16_t bind_port)
    : _is_connected(false),
      _buffer_max(1024 * 32),
      _encode_buffer(new FLAC__int32[_buffer_max]),
      _encode_buffer_size(new unsigned int(_buffer_max)),
      _send_buffer(new FLAC__byte[_buffer_max]),
      _send_buffer_size(new unsigned int) {
  // init ALSA capture device
  _capture = new ALSA::Capture();

  // init flac encoder
  _encoder_stream =
      new FLAC::Encoder(_encode_buffer, _encode_buffer_size, _send_buffer,
                        _send_buffer_size, _buffer_max);

  // init network socket
  if ((_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  int options = 0;
  if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options,
                 sizeof(options))) {
    FRIEND__THROWEXCEPTION;  // XXX
  }
  _bind_address.sin_family = AF_INET;
  _bind_address.sin_addr.s_addr = INADDR_ANY;  // XXX
  _bind_address.sin_port = htons(bind_port);

  if (bind(_socket_fd, (struct sockaddr*)&_bind_address,
           sizeof(_bind_address)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }
}

Input::~Input() {
  delete _capture;
  delete _encoder_stream;
  delete[] _encode_buffer;
  delete _encode_buffer_size;
  delete[] _send_buffer;
  delete _send_buffer_size;
}

void Input::_SetConnectedState(bool is_connected) {
  // XXX: set the output state of some LED
  _is_connected = is_connected;
}

void Input::MainLoop() {
#ifdef DEBUG
  std::cout << "_encode_buffer_size = " << *_encode_buffer_size << std::endl;
#endif
  // put data in the encode buffer
  _capture->CaptureSamples(_encode_buffer,
                           (snd_pcm_uframes_t)*_encode_buffer_size);

  // process data from the encode buffer and place it in the send buffer
  _encoder_stream->process_interleaved();

  // XXX: _client_address needs to be set.
  if (sendto(_socket_fd, _send_buffer, *_send_buffer_size, MSG_CONFIRM,
             &_client_address, sizeof(_client_address)) < 0) {
    FRIEND__THROWEXCEPTION;  // XXX
  }
}

}  // namespace Friend
