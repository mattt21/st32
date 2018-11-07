#include "Inverter.h"
DigitalOut AP(D3);
DigitalOut AN(D9);
DigitalOut BP(D8);
DigitalOut BN(D12);
DigitalOut CP(D6);
DigitalOut CN(D7);
Ticker pwm_ticker;

double TRIANGLE_INCRIMENT = .3;
double  SINE_OUT_FREQ   =  60;
double frequency = 22.0f;


// Number of elements in sampled sine wave
#define SINE_STEPS        400
// Frequency of output triangle in Hz
#define TRIANGE_FREQUENCY 440
// Number of steps per each Triangle Wave
#define TRIANGLE_STEPS  10

// Constants to compute the sine waveform
#define PI                 3.141592f
#define SINE_STEPS_RAD    (2.0f * PI / (float)SINE_STEPS)
#define SWM_STEPS         24
// Table to generate the sine waveform using dutycycles
float sine_duty[SINE_STEPS];

// Ticker calls this fucntion to update the PWM dutycycle
void pwm_duty_updater() {
  // counter is the triangle wave to compare the sine to
  static double counter = 0.0f;
  static int sine_index = 0;
  // Two vectors for the pins to open and close
  vector <DigitalOut> toZero;
  vector <DigitalOut> toOne;
  // Comparing the values for each PWM pin
  if (sine_duty[sine_index] >= counter && AP == 0) {
      toZero.push_back(AN);
      toOne.push_back(AP);
  }
  else if(AP == 1) {
      toZero.push_back(AP);
      toOne.push_back(AN);
  }

  if (sine_duty[(sine_index + SINE_STEPS/3)%SINE_STEPS] >= counter && BP == 0) {
      toZero.push_back(BN);
      toOne.push_back(BP);
  }
  else if(BP == 1) {
      toZero.push_back(BP);
      toOne.push_back(BN);
  }

  if (sine_duty[(sine_index + 2*SINE_STEPS/3)%SINE_STEPS] >= counter && CP == 0) {
      toZero.push_back(CN);
      toOne.push_back(CP);
  }
  else if(CP == 1) {
      toZero.push_back(CP);
      toOne.push_back(CN);
  }

  // Set the correct pins to zero
  for (int i = 0; i < toZero.size(); i++) {
    toZero[i] = 0;
  }
  // Wait the dead Time. I approximated it as 1000 nanoseconds or 1 microsecond
  wait_us(1);
  for (int i = 0; i < toOne.size(); i++) {
    toOne[i] = 1;
  }


  // Incriment the sine index and the counter
  sine_index++;
  counter += TRIANGLE_INCRIMENT;

  if (counter <= -1.5f || counter >= 1.5f) {
    TRIANGLE_INCRIMENT *= -1;
  }
  if (sine_index >= SINE_STEPS) {
    sine_index = 0;
  }
}
void initInverter() {
  int i;
  // Generate Sine Wave
  for (i=0; i<SINE_STEPS; i++) {
    sine_duty[i] = sin(i * SINE_STEPS_RAD);
  }
  //Attach Intterupt
  pwm_ticker.attach(&pwm_duty_updater, (1.0f / (float)(10.0f * TRIANGE_FREQUENCY)));

}

void changeMotorFrequency(double freq) {
  frequency = freq;
  if(freq>53.0f) {
    frequency = 53.0f;
  }

  pwm_ticker.detach();
  pwm_ticker.attach(&pwm_duty_updater, 1.0f/(10.0f*40.0f*frequency));
}
