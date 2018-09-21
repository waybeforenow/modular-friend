#ifndef _FRIEND_FLAC_DECODER_H
#define _FRIEND_FLAC_DECODER_H

#include "FLAC++/decoder.h"

namespace Friend {
namespace FLAC {

class Decoder : public ::FLAC::Decoder::Stream {
 private:
  unsigned char* _buffer;
  int* _read_bytes;

 public:
  Decoder(unsigned char* buffer, int* read_bytes)
      : _buffer(buffer), _read_bytes(read_bytes) {}

 protected:
  ::FLAC__StreamDecoderReadStatus read_callback(FLAC__byte buffer[],
                                                size_t* bytes);
  ::FLAC__StreamDecoderWriteStatus write_callback(
      const ::FLAC__Frame* frame, const FLAC__int32* const buffer[]);
  void error_callback(::FLAC__StreamDecoderErrorStatus status);
};

}  // namespace FLAC
}  // namespace Friend

#endif  // _FRIEND_FLAC_DECODER_H
