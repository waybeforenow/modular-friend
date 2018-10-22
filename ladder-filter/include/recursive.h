#ifndef __FRIEND_EFFECTS_LADDER_FILTER_LADDER_RECURSIVE_H
#define __FRIEND_EFFECTS_LADDER_FILTER_LADDER_RECURSIVE_H

#include <cmath>
#include <cstdint>
/* #include <limits> */

namespace Friend {
namespace Effects {
namespace LadderFilter {

class FilterConstants {
 private:
  float _r, _g, _Ts;

 public:
  FilterConstants(const uint16_t Fs, const uint16_t Fc, const float r)
      : _r(r),
        _g(1.f - std::exp(-2.f * M_PI * (const float)Fc / (const float)Fs)),
        _Ts(1.f / (float)Fs) {}
  ~FilterConstants() = default;

  // inline constexpr float Vt() const { return 0.000025; }
  inline constexpr float Vt() const { return 1.f; }
  inline constexpr float g() const { return _g; }
  inline constexpr float g2Vt() const { return 2.f * Vt() * g(); }
  inline constexpr float r() const { return _r; }
  inline constexpr float Ts() const { return _Ts; }
};

class FilterStage {
 private:
  const FilterConstants* c;
  float _last;
  FilterStage* _stageref;

  const float _tick(const float& n) {
    return _last + c->Ts() * c->g2Vt() *
                       (std::tanh(n / (2.f * c->Vt())) -
                        std::tanh(_last / (2.f * c->Vt())));
  }

 public:
  FilterStage(const FilterConstants* constants, FilterStage* stageref)
      : c(constants), _last(0.f), _stageref(stageref) {}

  const float tick(const float& n) {
    if (_stageref == nullptr) {
      return (_last = _tick(n));
    }

    return (_last = _tick(_stageref->tick(n)));
  }

  ~FilterStage() = default;
  const float get_last() { return _last; }
  FilterStage* get_stageref() { return _stageref; }
};

template <typename T>
class Filter {
 private:
  const FilterConstants* c;
  FilterStage* final_stage;
  const size_t n;

 public:
  Filter(float freq, float res, float sr = 44100, size_t poles = 4)
      : c(new FilterConstants(sr, freq, res)), final_stage(nullptr), n(poles) {
    for (size_t i = 0; i < n; i++) {
      final_stage = new FilterStage(c, final_stage);
    }
  }

  ~Filter() {
    while (final_stage != nullptr) {
      FilterStage* tmp = final_stage;
      final_stage = final_stage->get_stageref();
      delete tmp;
    }
  }

  /*
  static constexpr float pack5V(const T& n) {
    using limits = std::numeric_limits<T>;

    if (!limits::is_signed) {
      return (float)(n - (limits::max() / 2)) *
             (5.f / ((float)limits::max() / 2.f));
    }

    return (float)n * (5.f / (float)limits::max());
  }

  static constexpr T unpack5V(const float& n) {
    using limits = std::numeric_limits<T>;
    if (!limits::is_signed) {
      return (T)((n * ((float)limits::max() / 5.f)) + (limits::max() / 2));
    }

    return (T)(n * ((float)limits::max() / 5.f));
  }
  */

  const float tick(const T& n) {
    // return unpack5V(final_stage->tick(pack5V(n)));
    return final_stage->tick(n);
  }
};

}  // namespace LadderFilter
}  // namespace Effects
}  // namespace Friend

#endif  //  __FRIEND_EFFECTS_LADDER_FILTER_LADDER_RECURSIVE_H
