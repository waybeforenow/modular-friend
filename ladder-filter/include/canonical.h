/*
 * Portions of this code were adapted from the Soundpipe module `moogladder`[1],
 * which was adapted from the Csound opcode `moogladder`[2].
 * [1]
 * https://github.com/PaulBatchelor/Soundpipe/blob/master/modules/moogladder.c
 * [2] https://github.com/csound/examples/blob/master/opcode_demos/luamoog.csd
 *
 */

#ifndef __FRIEND_EFFECTS_LADDER_FILTER_LADDER_CANONICAL_H
#define __FRIEND_EFFECTS_LADDER_FILTER_LADDER_CANONICAL_H

#include <cmath>

namespace Friend {
namespace Effects {
namespace LadderFilter {

template <typename T>
class Filter {
 private:
  float* _freq;
  float* _res;
  float _sr;
  float _istor;
  float _delay[6];
  float _tanhstg[3];
  float _oldfreq;
  float _oldres;
  float _oldacr;
  float _oldtune;
  const float THERMAL;

  static double my_tanh(double x) {
    /* use the fact that tanh(-x) = - tanh(x)
     * and if x>~4 tanh is approx constant 1
     * and for small x tanh(x) =~ x
     * So giving a cheap approximation */
    int sign = 1;
    if (x < 0) {
      sign = -1;
      x = -x;
    }
    if (x >= 4.0) {
      return sign;
    }
    if (x < 0.5) return x * sign;
    return sign * tanh(x);
  }

 public:
  Filter(float* freq, float* res, float sr = 44100)
      : _freq(freq), _res(res), _sr(sr), _istor(0.0), THERMAL(0.000025) {
    int i;

    for (i = 0; i < 6; i++) _delay[i] = 0.0;
    for (i = 0; i < 3; i++) _tanhstg[i] = 0.0;
    _oldfreq = 0.0;
    _oldres = -1.0; /* ensure calculation on first cycle */
  }

  const float tick(const T& n) {
    float freq = *_freq;
    float res = *_res;
    float res4;
    float* delay = _delay;
    float* tanhstg = _tanhstg;
    float stg[4], input;
    float acr, tune;

    int j, k;
    if (res < 0) res = 0;
    if (_oldfreq != freq || _oldres != res) {
      float f, fc, fc2, fc3, fcr;
      _oldfreq = freq;
      /* sr is half the actual filter sampling rate  */
      fc = (float)(freq / _sr);
      f = 0.5 * fc;
      fc2 = fc * fc;
      fc3 = fc2 * fc;
      /* frequency & amplitude correction  */
      fcr = 1.8730 * fc3 + 0.4955 * fc2 - 0.6490 * fc + 0.9988;
      acr = -3.9364 * fc2 + 1.8409 * fc + 0.9968;
      tune =
          (1.0 - exp(-((2 * M_PI) * f * fcr))) / THERMAL; /* filter tuning  */
      _oldres = res;
      _oldacr = acr;
      _oldtune = tune;
    } else {
      res = _oldres;
      acr = _oldacr;
      tune = _oldtune;
    }
    res4 = 4.0 * (float)res * acr;
    /* oversampling  */
    for (j = 0; j < 2; j++) {
      /* filter stages  */
      input = n - res4 /*4.0*res*acr*/ * delay[5];
      delay[0] = stg[0] =
          delay[0] + tune * (my_tanh(input * THERMAL) - tanhstg[0]);
      for (k = 1; k < 4; k++) {
        input = stg[k - 1];
        stg[k] = delay[k] +
                 tune * ((tanhstg[k - 1] = my_tanh(input * THERMAL)) -
                         (k != 3 ? tanhstg[k] : my_tanh(delay[k] * THERMAL)));
        delay[k] = stg[k];
      }
      /* 1/2-sample delay for phase compensation  */

      delay[5] = (stg[3] + delay[4]) * 0.5;
      delay[4] = stg[3];
    }
    return (float)delay[5];
  }
};

}  // namespace LadderFilter
}  // namespace Effects
}  // namespace Friend

#endif  //  __FRIEND_EFFECTS_LADDER_FILTER_LADDER_CANONICAL_H
