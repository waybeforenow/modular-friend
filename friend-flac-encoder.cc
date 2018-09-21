#include "friend-flac-encoder.h"
#include <string.h>
#include "FLAC++/encoder.h"

namespace Friend {
namespace FLAC {

bool Encoder::process_interleaved() {
  return this->process_interleaved(_buffer, *_buffer_size);
}

::FLAC__StreamEncoderWriteStatus Encoder::write_callback(
    const FLAC__byte buffer[], size_t bytes, unsigned samples,
    unsigned current_frame) {
  size_t bytes_to_write =
      (_send_buffer_max_size > bytes) ? _send_buffer_max_size : bytes;
  memcpy(_send_buffer, buffer, bytes_to_write);
  *_send_buffer_size = bytes_to_write;
  if (bytes_to_write == _send_buffer_max_size) {
    // Yikes! We want to be able to spool the output from the encoder here, but
    // it's disallowed by the API. Best amelioration here is to make sure that
    // _send_buffer_max_size is large enough that it never overflows.

    return FLAC__STREAM_ENCODER_WRITE_STATUS_FATAL_ERROR;
  }

  return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
}

}  // namespace FLAC
}  // namespace Friend
