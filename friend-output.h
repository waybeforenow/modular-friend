#ifndef _FRIEND_OUTPUT_H
#define _FRIEND_OUTPUT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include "alsa/asoundlib.h"
#include "friend-alsa-playback.h"
#include "friend-flac-decoder.h"

namespace Friend {

class Output {
 private:
  ALSA::Playback* _playback;
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

}  // namespace Output

#endif  // _FRIEND_OUTPUT_H
