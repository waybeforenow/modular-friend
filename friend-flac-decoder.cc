#include "friend-flac-decoder.h"
#include <string.h>
#include "FLAC++/decoder.h"
#include "friend-defaults.h"

namespace Friend {
namespace FLAC {

::FLAC__StreamDecoderReadStatus Decoder::read_callback(FLAC__byte buffer[],
                                                       size_t *bytes) {
  bool buffer_will_underflow = (*bytes < *_decode_buffer_size);

  memcpy(buffer, _decode_buffer,
         (buffer_will_underflow) ? *bytes : *_decode_buffer_size);
  if (buffer_will_underflow) {
    *_decode_buffer_size -= *bytes;
    memmove(_decode_buffer, _decode_buffer + *bytes, *_decode_buffer_size);
    return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
  }

  *bytes = *_decode_buffer_size;
  return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
}

::FLAC__StreamDecoderWriteStatus Decoder::write_callback(
    const ::FLAC__Frame *frame, const FLAC__int32 *const buffer[]) {
  if (frame->header.channels != 2) {
    FRIEND__THROWEXCEPTION;  // XXX
  }

  size_t bytes_to_copy =
      frame->header.blocksize * (frame->header.bits_per_sample / 8);
  if (_playback_buffer_max_size < bytes_to_copy) {
    // since the decoder writer won't spool for us, we need to make sure this
    // condition is never reached

    return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
  }

  memcpy(_playback_left_buffer, buffer[0], bytes_to_copy);
  memcpy(_playback_right_buffer, buffer[1], bytes_to_copy);
  *_playback_buffer_size = bytes_to_copy;

  return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;  // XXX
}

void Decoder::error_callback(::FLAC__StreamDecoderErrorStatus status) {}  // XXX

}  // namespace FLAC
}  // namespace Friend
