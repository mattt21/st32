#include "mbed.h"
#include "FastPWM.h"
#include <map>
#include <string>

using namespace std;
// Number of dutycycle steps for output wave
#define SINE_STEPS        32
// Frequency of output sine in Hz
#define SINE_OUT_FREQ     60

// Constants to compute the sine waveform
#define PI                 3.141592f
#define SINE_STEPS_RAD    (2.0f * PI / (float)SINE_STEPS)
#define PWM_FREQ 10000
#define BOOST_FREQ 100000
// Table to generate the sine waveform using dutycycles
float sine_duty[SINE_STEPS];

AnalogIn DCCurrentIn (A1);
AnalogIn DCVoltageIn (A2);

FastPWM AP (D3);
FastPWM AN (D9);
FastPWM BP (D8);
FastPWM BN (D12);
FastPWM CP (D6);
FastPWM CN (D10);
FastPWM Boost(A0);
//Heartbeat LED
DigitalOut myled(LED1);

//Ticker to update the PWM dutycycle
Ticker pwm_ticker;
Ticker pwm_tickerN;
Ticker boost_ticker;

// Ticker calls this fucntion to update the PWM dutycycle
void pwm_duty_updater() {
  static int idx=0;
  static int idx2=10;
  static int idx3=22;

  AP.write(sine_duty[idx]);  // Set the dutycycle % to next value in array
  BP.write(sine_duty[idx2]);  // Set the dutycycle % to next value in array
  CP.write(sine_duty[idx3]);  // Set the dutycycle % to next value in array

  idx++; idx2++; idx3++;                        // Increment the idx
  if (idx == SINE_STEPS) idx=0;  // Reset the idx when teh end has been reached
  if (idx2 == SINE_STEPS) idx2=0;
  if (idx3 == SINE_STEPS) idx3=0;

}

void pwm_duty_updaterN() {
  static int idx=0;
  static int idx2=10;
  static int idx3=22;

  AN.write(sine_duty[SINE_STEPS-idx-1]);  // Set the dutycycle % to next value in array
  BN.write(sine_duty[SINE_STEPS-idx2-1]);  // Set the dutycycle % to next value in array
  CN.write(sine_duty[SINE_STEPS-idx3-1]);  // Set the dutycycle % to next value in array

  idx++; idx2++; idx3++;                        // Increment the idx
  if (idx == SINE_STEPS) idx=0;  // Reset the idx when teh end has been reached
  if (idx2 == SINE_STEPS) idx2=0;
  if (idx3 == SINE_STEPS) idx3=0;

}
double getPanelVoltage(){
  return DCVoltageIn.read();
}
double getPanelCurrent() {
  return DCCurrentIn.read();

}

double calculateDutyCycle()
{
  /*
  static double oldVoltage = 0;
  static double oldCurrent = 0;
  static double dutyCycle = 0.2;
  double deltaB = 0.001;
  double currentVoltage = getPanelVoltage();
  double currentCurrent = getPanelCurrent();

  double currentPower = currentCurrent*currentPower;
  double pastPower = oldVoltage*oldCurrent;

  if (pastPower > currentPower) {
    if (oldVoltage > currentVoltage) {
    dutyCycle -= deltaB;
  }
    else {
    dutyCycle += deltaB;
  }
  }
  else {
  if (oldVoltage > currentVoltage) {
  dutyCycle += deltaB;
  }
  else {
  dutyCycle -= deltaB;
  }

  }
  oldVoltage = currentVoltage;
  oldCurrent = currentCurrent;

  if (dutyCycle >= 0.8)
  {
  dutyCycle = 0.8;
}
  if (dutyCycle <= 0.2) {
  dutyCycle = 0.2;
}
  return dutyCycle;

  */
  return 0.2;
}

void boostUpdater()
{
  static double dutyCycle = calculateDutyCycle();
  Boost.write(dutyCycle);
}



int main() {
  int i;

  // Init the duty cycle array
  for (i=0; i<SINE_STEPS; i++) {
    sine_duty[i] = ( sin(i * SINE_STEPS_RAD) + 1.0f ) / 2.0f;  // convert sine (-1.0 .. +1.0) into dutycycle (0.0 .. 1.0)
  }

  // Init the Ticker to call the dutycyle updater at the required interval
  // The update should be at (SINE_STEPS * SINE_OUT_FREQ)
  AP.period( 1.0f / (float) PWM_FREQ);
  AN.period( 1.0f / (float) PWM_FREQ);
  BP.period( 1.0f / (float) PWM_FREQ);
  BN.period( 1.0f / (float) PWM_FREQ);
  CP.period( 1.0f / (float) PWM_FREQ);
  CN.period( 1.0f / (float) PWM_FREQ);
  Boost.period( 1.0f/ (float) BOOST_FREQ);

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
