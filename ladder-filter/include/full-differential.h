#ifndef __FRIEND_EFFECTS_LADDER_FILTER_LADDER_FULL_DIFFERENTIAL_H
#define __FRIEND_EFFECTS_LADDER_FILTER_LADDER_FULL_DIFFERENTIAL_H

#include <cmath>
#include <cstdint>

namespace Friend {
namespace Effects {
namespace LadderFilter {

class FilterConstants {
 private:
  float _r, _Vt, _g;

 public:
  FilterConstants(const uint16_t Fs, const uint16_t Fc, const float r)
      : _r(r),
        _Vt(0.000025),
        _g(2000.f * M_PI * (const float)Fc / (const float)Fs) {}
  ~FilterConstants() = default;

  constexpr inline const float Vt() const { return _Vt; }
  constexpr inline const float g() const { return _g; }
  constexpr inline const float r() const { return _r; }
};

template <typename T>
class FilterStage {
 protected:
  const FilterConstants* c;
  FilterStage<T>* _stageref;
  float _last;
  float _lastW;
  FilterStage(const FilterConstants* constants)
      : c(constants), _stageref(nullptr), _last(0.f), _lastW(0.f) {}
  virtual const float _tick(const T&) = 0;

 public:
  virtual const float tickW(const T& n) {
    return (_lastW = std::tanh(_tick(n) / (2.f * c->Vt())));
  }

  virtual const float tick(const T& n) { return (_last = _tick(n)); }

  void set_stageref(FilterStage* other_stage) { _stageref = other_stage; }
  virtual ~FilterStage() = default;
  const float get_last() { return _last; }
};

template <typename T>
class FilterStageA : public FilterStage<T> {
 public:
  FilterStageA(const FilterConstants* constants) : FilterStage<T>(constants) {}

 protected:
  virtual const float _tick(const T& n) {
    return this->_last +
           this->c->g() *
               (std::tanh((const float)n - 4.f * this->c->r() *
                                               this->_stageref->get_last() /
                                               (2.f * this->c->Vt())) -
                this->_lastW);
  }
};

template <typename T>
class FilterStageB : public FilterStage<T> {
 public:
  FilterStageB(const FilterConstants* constants) : FilterStage<T>(constants) {}

 protected:
  virtual const float _tick(const T& n) {
    return this->_last +
           this->c->g() *
               (this->_stageref->tickW((const float)n) - this->_lastW);
  }
};

template <typename T>
class FilterStageC : public FilterStage<T> {
 public:
  FilterStageC(const FilterConstants* constants) : FilterStage<T>(constants) {}

 protected:
  virtual const float _tick(const T& n) {
    return this->_last +
           this->c->g() * (this->_stageref->tickW(n) - this->_lastW);
  }
};

template <typename T>
class FilterStageD : public FilterStage<T> {
 public:
  FilterStageD(const FilterConstants* constants) : FilterStage<T>(constants) {}

 protected:
  virtual const float _tick(const T& n) {
    return this->_last +
           this->c->g() * (this->_stageref->tickW(n) -
                           std::tanh(this->_last / (2.f * this->c->Vt())));
  }
};

template <typename T>
class Filter {
 private:
  const FilterConstants* c;
  FilterStageA<T>* stageA;
  FilterStageB<T>* stageB;
  FilterStageC<T>* stageC;
  FilterStageD<T>* stageD;

 public:
  Filter(float freq, float res, float sr = 44100)
      : c(new FilterConstants(sr, freq, res)),
        stageA(new FilterStageA<T>(c)),
        stageB(new FilterStageB<T>(c)),
        stageC(new FilterStageC<T>(c)),
        stageD(new FilterStageD<T>(c)) {
    stageA->set_stageref(stageD);
    stageB->set_stageref(stageA);
    stageC->set_stageref(stageB);
    stageD->set_stageref(stageC);
  }

  const float tick(const T& n) { return stageD->tick(n); }
};

}  // namespace LadderFilter
}  // namespace Effects
}  // namespace Friend

#endif  //  __FRIEND_EFFECTS_LADDER_FILTER_LADDER_FULL_DIFFERENTIAL_H
