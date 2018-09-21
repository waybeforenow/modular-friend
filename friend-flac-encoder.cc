#include "friend-flac-encoder.h"
#include <string.h>
#include "FLAC++/encoder.h"

namespace Friend {
namespace FLAC {

::FLAC__StreamEncoderReadStatus Encoder::read_callback(FLAC__byte buffer[],
                                                       size_t *bytes) {
  bool buffer_will_overflow = (*_buffer_size > *bytes);
  memcpy(buffer, _buffer, (buffer_will_overflow) ? *bytes : *_buffer_size);
  if (buffer_will_overflow) {
    *_buffer_size -= *bytes;
    memmove(_buffer, _buffer + *bytes, *_buffer_size);
    return FLAC__STREAM_ENCODER_READ_STATUS_CONTINUE;
  }

  return FLAC__STREAM_ENCODER_READ_STATUS_END_OF_STREAM;
}

::FLAC__StreamEncoderWriteStatus Encoder::write_callback(
    const FLAC__byte buffer[], size_t bytes, unsigned samples,
    unsigned current_frame) {
  return FLAC__STREAM_ENCODER_WRITE_STATUS_OK; // XXX
}

}  // namespace FLAC
}  // namespace Friend
