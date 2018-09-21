#include "friend-output.h"
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>

namespace Friend {

Output::Output(struct sockaddr_in address) : _address(address), _is_connected(false), _buffer_size(1024 * 32), _buffer(new unsigned char[_buffer_size]), _read_bytes(new int) {
  // XXX: init alsa

  // XXX: init flac decoder
  _decoder_stream = new FLAC::Decoder(_buffer, _read_bytes);

  if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    throw std::exception(); // XXX
  }

  if (connect(_socket_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
    throw std::exception(); // XXX
  }

  _SetConnectedState(true); // XXX wait for heartbeat from server
}

void Output::_SetConnectedState(bool is_connected) {
  // XXX: set the output state of some LED
  _is_connected = is_connected;
}

void Output::MainLoop() {
  int bytes_to_read = 0;
  ioctl(_socket_fd, FIONREAD, &bytes_to_read);
  if (bytes_to_read > _buffer_size) {
    *_read_bytes = read(_socket_fd, _buffer, _buffer_size);
  } else {
    *_read_bytes = read(_socket_fd, _buffer, bytes_to_read);
  }
}

} // namespace Friend

int main() { return 0; }
