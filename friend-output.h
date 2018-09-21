#ifndef _FRIEND_OUTPUT_H
#define _FRIEND_OUTPUT_H

#include "friend-flac-decoder.h"
#include <sys/socket.h>
#include <netinet/in.h>

namespace Friend {

class Output {
 private:
  // snd_pcm_t* _pcm_handle;
  int _socket_fd;
  struct sockaddr_in _address;
  bool _is_connected;
  void _SetConnectedState(bool is_connected);
  const int _buffer_size;
  unsigned char* _buffer;
  int* _read_bytes;
  FLAC::Decoder* _decoder_stream;

 public:
  Output(sockaddr_in address);
  void MainLoop();
};

} // namespace Output

#endif // _FRIEND_OUTPUT_H
