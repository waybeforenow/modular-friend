#ifndef __FRIEND_DEFAULTS_H
#define __FRIEND_DEFAULTS_H

#include "friend-exceptions.h"

#ifndef FRIEND__SAMPLE_RATE
#define FRIEND__SAMPLE_RATE 44100
#endif

#ifndef FRIEND__ALSA_DEVICE_NAME
#define FRIEND__ALSA_DEVICE_NAME "plughw:CARD=Audio,DEV=0"
#endif

#ifndef FRIEND__PCM_FORMAT
#define FRIEND__PCM_FORMAT SND_PCM_FORMAT_FLOAT_LE
#endif

#ifndef FRIEND__PCM_TYPE
#define FRIEND__PCM_TYPE float
#endif


#define FRIEND__THROWEXCEPTION throw Friend::runtime_error(__FILE__, __LINE__)
#define FRIEND__THROWEXCEPTIONWITHTEXT(x) \
  throw Friend::runtime_error(x, __FILE__, __LINE__)

#endif  // __FRIEND_DEFAULTS_H
