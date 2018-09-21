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
  size_t _buffer_max;
  FLAC__byte* _decode_buffer;
  unsigned int* _decode_buffer_size;
  FLAC__int32* _playback_left_buffer;
  FLAC__int32* _playback_right_buffer;
  unsigned int* _playback_buffer_size;
  FLAC::Decoder* _decoder_stream;

 public:
  Output(sockaddr_in address);
  void MainLoop();
};

}  // namespace Output

#endif  // _FRIEND_OUTPUT_H
