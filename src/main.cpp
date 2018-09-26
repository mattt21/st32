

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


//Frequency of Pulse Width Modulated signal in Hz
#define PWM_FREQ          10000
#define BOOST_FREQ        100000

//PWM pin
FastPWM PwmPinAN (D3);
FastPWM PwmPinAP (D9);
FastPWM PwmPinBP (D8);
FastPWM PwmPinBN (D12);
FastPWM PwmPinCP (D6);
FastPWM PwmPinCN (D10);
FastPWM PwmBoost(A0);
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

  PwmPinAP.write(sine_duty[idx]);  // Set the dutycycle % to next value in array
  PwmPinBP.write(sine_duty[idx2]);  // Set the dutycycle % to next value in array
  PwmPinCP.write(sine_duty[idx3]);  // Set the dutycycle % to next value in array

  idx++; idx2++; idx3++;                        // Increment the idx
  if (idx == SINE_STEPS) idx=0;  // Reset the idx when teh end has been reached
  if (idx2 == SINE_STEPS) idx2=0;
  if (idx3 == SINE_STEPS) idx3=0;

}

void pwm_duty_updaterN() {
  static int idx=0;
  static int idx2=10;
  static int idx3=22;

  PwmPinAN.write(sine_duty[31-idx]);  // Set the dutycycle % to next value in array
  PwmPinBN.write(sine_duty[31-idx2]);  // Set the dutycycle % to next value in array
  PwmPinCN.write(sine_duty[31-idx3]);  // Set the dutycycle % to next value in array

  idx++; idx2++; idx3++;                        // Increment the idx
  if (idx == SINE_STEPS) idx=0;  // Reset the idx when teh end has been reached
  if (idx2 == SINE_STEPS) idx2=0;
  if (idx3 == SINE_STEPS) idx3=0;

}

void boostUpdater()
{
  double dutyCycle = .5;
  PwmBoost.write(dutyCycle);

}

int main() {
  int i;

  // Init the duty cycle array
  for (i=0; i<SINE_STEPS; i++) {
    sine_duty[i] = ( sin(i * SINE_STEPS_RAD) + 1.0f ) / 2.0f;  // convert sine (-1.0 .. +1.0) into dutycycle (0.0 .. 1.0)
  }
  // Set PWM frequency to 200 KHz (period = 5 us)
  PwmPinAP.period( 1.0f / (float) PWM_FREQ);
  PwmPinAN.period( 1.0f / (float) PWM_FREQ);
  PwmPinBP.period( 1.0f / (float) PWM_FREQ);
  PwmPinBN.period( 1.0f / (float) PWM_FREQ);
  PwmPinCP.period( 1.0f / (float) PWM_FREQ);
  PwmPinCN.period( 1.0f / (float) PWM_FREQ);
  PwmBoost.period( 1.0f/ (float) BOOST_FREQ);


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
