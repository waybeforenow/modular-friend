#include "friend-flac-decoder.h"
#include "FLAC++/decoder.h"
#include <string.h>

namespace Friend {
namespace FLAC {

::FLAC__StreamDecoderReadStatus Decoder::read_callback (FLAC__byte buffer[], size_t *bytes) {
  bool buffer_will_overflow = (*bytes > *_read_bytes);

  memcpy(buffer, _buffer, (buffer_will_overflow) ? *bytes : *_read_bytes);
  if (buffer_will_overflow) {
    *_read_bytes -= *bytes;
    memmove(_buffer, _buffer+*bytes, *_read_bytes);
    return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
  } 

  return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
}

::FLAC__StreamDecoderWriteStatus Decoder::write_callback(const ::FLAC__Frame *frame, const FLAC__int32 *const buffer[]) {
  return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE; // XXX
}

void Decoder::error_callback(::FLAC__StreamDecoderErrorStatus status) {} // XXX

} // namespace FLAC 
} // namespace Friend

