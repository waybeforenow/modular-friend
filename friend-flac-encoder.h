#ifndef _FRIEND_FLAC_ENCODER_H
#define _FRIEND_FLAC_ENCODER_H

#include "FLAC++/encoder.h"

namespace Friend {
namespace FLAC {

class Encoder : public ::FLAC::Encoder::Stream {
 private:
  unsigned char* _buffer;
  int* _buffer_size;

 public:
  Encoder(unsigned char* buffer, int* buffer_size)
      : _buffer(buffer), _buffer_size(buffer_size) {}

 protected:
  ::FLAC__StreamEncoderReadStatus read_callback(FLAC__byte buffer[],
                                                size_t* bytes);
  ::FLAC__StreamEncoderWriteStatus write_callback(const FLAC__byte buffer[],
                                                  size_t bytes,
                                                  unsigned samples,
                                                  unsigned current_frame);
};

}  // namespace FLAC
}  // namespace Friend

#endif  // _FRIEND_FLAC_ENCODER_H
