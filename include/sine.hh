#ifndef SINE_H_
#define SINE_H_

#include <cmath>
#include "approx.hh"

namespace snd {

template <class fp_t>
class Sine {
 public:
  Sine(fp_t sampleRate) {
    SR = sampleRate;
    phase = frequency = frequencyState = 0;
    increment = sineStep = out = latency = 0;
  };
  ~Sine() {};

  fp_t tick() {
    this->_interpolateFrequency();
    latency = out;
    this->_triangularDriver();
    out = sineApprox7odd<fp_t>(sineStep);
    phase -= increment;
    fp_t sign = -1;
    if (0.5 < std::abs(phase)) {
      if (phase > 0) {
        sign = 1;
      }
      phase -= sign;
    }
    return latency;
  }

  void setFrequency(fp_t frequency) {
    this->frequency = frequency;
  }

  void setPhase(fp_t phase) {
    fp_t phs = phase;
    phs -= 0.5;
    phs -= std::round(phs);
    this->phase = phs;
  }

 private:
  fp_t SR;
  fp_t frequency;
  fp_t phase, frequencyState, increment, latency, sineStep, out;

  void _interpolateFrequency() {
    increment = frequency / SR;
    increment = increment > 0.5 ? 0.5 : (increment < -0.5 ? -0.5 : increment);
    frequencyState = frequency;
  }

  void _triangularDriver() {
    sineStep = phase - 0.25;
    sineStep -= std::round(sineStep);
    sineStep += sineStep;
    sineStep = std::abs(sineStep);
    sineStep = 0.5 - sineStep;
  }
};

}; // !namespace snd

#endif // !SINE_H
