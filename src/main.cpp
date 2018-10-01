#include "mbed.h"
#include "FastPWM.h"
//Number of dutycycle steps for output wave
#define SINE_STEPS        32
//Frequency of output sine in Hz
#define SINE_OUT_FREQ     60

//Constants to compute the sine waveform
#define PI                 3.141592f
#define SINE_STEPS_RAD    (2.0f * PI / (float)SINE_STEPS)

//Table to generate the sine waveform using dutycycles
float sine_duty[SINE_STEPS];

class PwmPins{

private:
  //Pinout
  FastPWM pin;
  //Frequency of Pulse Width Modulated signal in Hz
  double freq;
  //Clock speed
  double clock;

public:
  PwmPins(FastPWM _pin, double _freq, double _clock):
  pin(_pin),
  freq(_freq),
  clock(_clock)
  { SetFreq(); }

  void SetFreq(){
    pin.period((1.0f / (float) freq));
  }
  void SetDuty(float _duty){
    pin.write(_duty);
  }


private:

};

//PWM pinout mapping
std::map <std::string, PwmPins> PwmPin;

// Set PWM frequency to 200 KHz (period = 5 us)
PwmPin["D3"] = PwmPins(D3, 10000, 16000000);
PwmPin["D9"] = PwmPins(D9, 10000, 16000000),
PwmPin["D8"] = PwmPins(D8, 10000, 16000000);
PwmPin["D12"] = PwmPins(D12, 10000, 16000000);
PwmPin["D6"] = PwmPins(D6, 10000, 16000000);
PwmPin["D10"] = PwmPins(D10, 10000, 16000000),
PwmPin["A0"] = PwmPins(A0, 100000, 16000000);

//Heartbeat LED
DigitalOut myled(LED1);

//Ticker to update the PWM dutycycle
Ticker pwm_ticker;
Ticker pwm_tickerN;
Ticker boost_ticker;

//Ticker calls this fucntion to update the PWM dutycycle
void pwm_duty_updater() {
  static int idx=0;
  static int idx2=10;
  static int idx3=22;

  PwmPin["D9"].SetDuty(sine_duty[idx]);  // Set the dutycycle % to next value in array
  PwmPin["D8"].SetDuty(sine_duty[idx]);  // Set the dutycycle % to next value in array
  PwmPin["D6"].SetDuty(sine_duty[idx]);  // Set the dutycycle % to next value in array

  idx++; idx2++; idx3++;                        // Increment the idx
  if (idx == SINE_STEPS) idx=0;  // Reset the idx when teh end has been reached
  if (idx2 == SINE_STEPS) idx2=0;
  if (idx3 == SINE_STEPS) idx3=0;

}

void pwm_duty_updaterN() {
  static int idx=0;
  static int idx2=10;
  static int idx3=22;

  PwmPin["D3"].SetDuty(sine_duty[31-idx]);  // Set the dutycycle % to next value in array
  PwmPin["D9"].SetDuty(sine_duty[31-idx2]);  // Set the dutycycle % to next value in array
  PwmPin["D8"].SetDuty(sine_duty[31-idx3]);  // Set the dutycycle % to next value in array

  idx++; idx2++; idx3++;                        // Increment the idx
  if (idx == SINE_STEPS) idx=0;  // Reset the idx when teh end has been reached
  if (idx2 == SINE_STEPS) idx2=0;
  if (idx3 == SINE_STEPS) idx3=0;

}

void boostUpdater()
{
  double dutyCycle = .5;
  PwmPin["A0"].SetDuty(dutyCycle);
}

int main() {
  int i;

  // Init the duty cycle array
  for (i=0; i<SINE_STEPS; i++) {
    sine_duty[i] = ( sin(i * SINE_STEPS_RAD) + 1.0f ) / 2.0f;  // convert sine (-1.0 .. +1.0) into dutycycle (0.0 .. 1.0)
  }

  // Init the Ticker to call the dutycyle updater at the required interval
  // The update should be at (SINE_STEPS * SINE_OUT_FREQ)

  __disable_irq();
  pwm_ticker.attach(&pwm_duty_updater, 1.0f / (float)(SINE_STEPS * SINE_OUT_FREQ));
  pwm_tickerN.attach(&pwm_duty_updaterN, 1.0f / (float)(SINE_STEPS * SINE_OUT_FREQ));
  boost_ticker.attach(&boostUpdater, 2);
  __enable_irq();

  while(1){ //infinite loop
    myled = !myled;
    wait(0.5);
  }

}
