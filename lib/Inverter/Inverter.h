#ifndef INVERTER_H
#define INVERTER_H
#include "mbed.h"
#include <vector>
// Number of dutycycle steps for output wave

inline double TRIANGLE_INCRIMENT = .3;
inline double  SINE_OUT_FREQ   =  60;

inline Ticker pwm_ticker;
inline int sine_index = 0;
inline void pwm_duty_updater();
void initInverter();

#endif
