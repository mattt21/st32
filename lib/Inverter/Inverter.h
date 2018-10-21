#ifndef INVERTER_H
#define INVERTER_H
#include "mbed.h"
#include <vector>
// Number of dutycycle steps for output wave
#define SINE_STEPS        750
// Frequency of output sine in Hz
double  SINE_OUT_FREQ   =  60;

#define TRIANGE_FREQUENCY 12000

#define TRIANGLE_STEPS  10

double TRIANGLE_INCRIMENT = .3;
// Constants to compute the sine waveform
#define PI                 3.141592f
#define SINE_STEPS_RAD    (2.0f * PI / (float)SINE_STEPS)
#define SWM_STEPS         24
// Table to generate the sine waveform using dutycycles
float sine_duty[SINE_STEPS];

DigitalOut AP(D3);
DigitalOut AN(D9);
DigitalOut BP(D8);
DigitalOut BN(D12);
DigitalOut CP(D6);
DigitalOut CN(D7);

Ticker pwm_ticker;
int sine_index = 0;
void pwm_duty_updater();

#endif
