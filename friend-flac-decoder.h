#ifndef __FRIEND_FLAC_DECODER_H
#define __FRIEND_FLAC_DECODER_H

#include "FLAC++/decoder.h"

namespace Friend {
namespace FLAC {

class Decoder : public ::FLAC::Decoder::Stream {
 private:
  FLAC__byte* _decode_buffer;
  unsigned int* _decode_buffer_size;
  FLAC__int32* _playback_left_buffer;
  FLAC__int32* _playback_right_buffer;
  unsigned int* _playback_buffer_size;
  size_t _playback_buffer_max_size;

 public:
  Decoder(FLAC__byte* decode_buffer, unsigned int* decode_buffer_size,
          FLAC__int32* playback_left_buffer, FLAC__int32* playback_right_buffer,
          unsigned int* playback_buffer_size, size_t playback_buffer_max_size)
      : _decode_buffer(decode_buffer),
        _decode_buffer_size(decode_buffer_size),
        _playback_left_buffer(playback_left_buffer),
        _playback_right_buffer(playback_right_buffer),
        _playback_buffer_size(playback_buffer_size),
        _playback_buffer_max_size(playback_buffer_max_size) {}

 protected:
  ::FLAC__StreamDecoderReadStatus read_callback(FLAC__byte buffer[],
                                                size_t* bytes);
  ::FLAC__StreamDecoderWriteStatus write_callback(
      const ::FLAC__Frame* frame, const FLAC__int32* const buffer[]);
  void error_callback(::FLAC__StreamDecoderErrorStatus status);
};

}  // namespace FLAC
}  // namespace Friend

#endif  // __FRIEND_FLAC_DECODER_H
