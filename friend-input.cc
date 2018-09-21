#include "friend-input.h"
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include "alsa/asoundlib.h"

namespace Friend {

Input::Input(uint16_t bind_port)
    : _is_connected(false),
      _buffer_max(1024 * 32),
      _encode_buffer(new unsigned char[_buffer_max]),
      _encode_buffer_size(new int),
      _send_buffer(new unsigned char[_buffer_max]),
      _send_buffer_size(new int) {
  // XXX: init alsa

  // init flac encoder
  _encoder_stream = new FLAC::Encoder(_encode_buffer, _encode_buffer_size);

  // init network socket
  if ((_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    throw std::exception();  // XXX
  }

  int options = 0;
  if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options,
                 sizeof(options))) {
    throw std::exception();  // XXX
  }
  _bind_address.sin_family = AF_INET;
  _bind_address.sin_addr.s_addr = INADDR_ANY;  // XXX
  _bind_address.sin_port = htons(bind_port);

  if (bind(_socket_fd, (struct sockaddr*)&_bind_address,
           sizeof(_bind_address)) < 0) {
    throw std::exception();  // XXX
  }
}

void Input::_SetConnectedState(bool is_connected) {
  // XXX: set the output state of some LED
  _is_connected = is_connected;
}

void Input::MainLoop() {
  // XXX: put data in the buffer

  if (sendto(_socket_fd, _send_buffer, *_send_buffer_size, MSG_CONFIRM,
             &_client_address, sizeof(_client_address)) < 0) {
    throw std::exception();  // XXX
  }
}

}  // namespace Friend