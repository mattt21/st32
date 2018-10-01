#include "FastPWM.h"
#include "mbed.h"

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
