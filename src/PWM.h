// Copyright (c) 
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "FastPWM.h"
#include "mbed.h"

#ifndef PWM_PINOUT
#define PWM_PINOUT

class PWMPinout{

private:
  //Pinout
  FastPWM pin;
  //Frequency of Pulse Width Modulated signal in Hz
  double freq;
  //Clock speed
  double clock;

public:
  PWMPinout(FastPWM _pin, double _freq, double _clock);

  void SetFreq();

  void SetDuty(float _duty);

};

#endif
