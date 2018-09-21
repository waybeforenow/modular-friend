#ifndef _FRIEND_FLAC_ENCODER_H
#define _FRIEND_FLAC_ENCODER_H

#include "FLAC++/encoder.h"

namespace Friend {
namespace FLAC {

class Encoder : public ::FLAC::Encoder::Stream {
 private:
  FLAC__int32* _buffer;
  unsigned int* _buffer_size;
  FLAC__byte* _send_buffer;
  unsigned int* _send_buffer_size;
  unsigned int _send_buffer_max_size;

 public:
  Encoder(FLAC__int32* buffer, unsigned int* buffer_size,
          FLAC__byte* send_buffer, unsigned int* send_buffer_size,
          unsigned int send_buffer_max_size)
      : _buffer(buffer),
        _buffer_size(buffer_size),
        _send_buffer(send_buffer),
        _send_buffer_size(send_buffer_size),
        _send_buffer_max_size(send_buffer_max_size) {}

  using ::FLAC::Encoder::Stream::process_interleaved;
  bool process_interleaved();

 protected:
  ::FLAC__StreamEncoderWriteStatus write_callback(const FLAC__byte buffer[],
                                                  size_t bytes,
                                                  unsigned samples,
                                                  unsigned current_frame);
};

}  // namespace FLAC
}  // namespace Friend

#endif  // _FRIEND_FLAC_ENCODER_H
