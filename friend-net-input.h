#ifndef __FRIEND_INPUT_H
#define __FRIEND_INPUT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include "alsa/asoundlib.h"
#include "friend-alsa-capture.h"
#include "friend-flac-encoder.h"

namespace Friend {

class Input {
 private:
  ALSA::Capture* _capture;
  struct sockaddr_in _bind_address;
  struct sockaddr _client_address;
  int _socket_fd;
  bool _is_connected;
  void _SetConnectedState(bool is_connected);
  const unsigned int _buffer_max;
  FLAC__int32* _encode_buffer;
  unsigned int* _encode_buffer_size;
  FLAC__byte* _send_buffer;
  unsigned int* _send_buffer_size;
  FLAC::Encoder* _encoder_stream;

 public:
  Input(uint16_t bind_port);
  ~Input();
  void MainLoop();
};

}  // namespace Friend

#endif  // __FRIEND_INPUT_H