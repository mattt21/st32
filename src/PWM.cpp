#include "PWM.h"

PWMPinout::PWMPinout(FastPWM _pin, double _freq, double _clock):
pin(_pin),
freq(_freq),
clock(_clock)
{
  SetFreq();
}

void PWMPinout::SetFreq(){
  pin.period((1.0f / (float) freq));
}
void PWMPinout::SetDuty(float _duty){
  pin.write(_duty);
}
