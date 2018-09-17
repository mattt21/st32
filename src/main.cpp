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
#define PWM_FREQ          2000
#define CLOCK_SPEED       16000000
//PWM pin
PwmOut PwmPin (D11);
PwmOut PwmPin2 (D6);
PwmOut PwmPin3 (D15);

//Heartbeat LED
DigitalOut myled(LED1);

Serial pc(USBTX, USBRX);
//Ticker to update the PWM dutycycle
Ticker pwm_ticker;

//Ticker calls this fucntion to update the PWM dutycycle
void pwm_duty_updater() {
  static int idx=0;
  static int idx2 = 10;
  static int idx3 = 20;
  PwmPin.write((sine_duty[idx]));  // Set the dutycycle % to next value in array
  PwmPin2.write((sine_duty[idx2]));  // Set the dutycycle % to next value in array
  PwmPin3.write((sine_duty[idx3]));  // Set the dutycycle % to next value in array
  idx++;                         // Increment the idx
  idx2++;
  idx3++;
  if (idx == SINE_STEPS) idx=0;  // Reset the idx when teh end has been reached
  if(idx2 == SINE_STEPS) idx2=0;
  if(idx3 == SINE_STEPS) idx3=0;

}

int main() {
  int i;

  // Init the duty cycle array
  for (i=0; i<SINE_STEPS; i++) {
    sine_duty[i] = ( sin(i * SINE_STEPS_RAD) + 1.0f ) / 2.0f;  // convert sine (-1.0 .. +1.0) into dutycycle (0.0 .. 1.0)
  }

  // Set PWM frequency to 200 KHz (period = 5 us)
  PwmPin.period( (1.0f / (float) PWM_FREQ));
  PwmPin2.period( (1.0f / (float) PWM_FREQ));
  PwmPin3.period( (1.0f / (float) PWM_FREQ));


  // Init the Ticker to call the dutycyle updater at the required interval
  // The update should be at (SINE_STEPS * SINE_OUT_FREQ)
  pwm_ticker.attach(&pwm_duty_updater, 1.0f / (float)(SINE_STEPS * SINE_OUT_FREQ));

  while(1){ //infinite loop
    myled = !myled;
    wait(0.5);
  }

}
