#ifndef INVERTER_H
#define INVERTER_H
#include "mbed.h"
#include "main.h"
#include "rtos.h"
#include <vector>
// Number of dutycycle steps for output wave

void pwm_duty_updater();

void initInverter();

void changeMotorFrequency(double freq);

void updateMotorFrequency(double freq);

extern double frequency;
//extern Mutex frequencyLock;


#endif
