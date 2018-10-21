#include "mbed.h"
#include "rtos.h"
#include "FastPWM.h"
#include <vector>
// Number of dutycycle steps for output wave
#define SINE_STEPS        750
// Frequency of output sine in Hz
#define SINE_OUT_FREQ     60

#define TRIANGE_FREQUENCY 12000

#define TRIANGLE_STEPS  10

double TRIANGLE_INCRIMENT = .1;
// Constants to compute the sine waveform
#define PI                 3.141592f
#define SINE_STEPS_RAD    (2.0f * PI / (float)SINE_STEPS)
#define SWM_STEPS         24
// Table to generate the sine waveform using dutycycles
float sine_duty[SINE_STEPS];

float swm_duty[SWM_STEPS] = {0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15,
  0.15, 0.325, 0.50, 0.675, 0.85, 0.85, 0.85, 0.85, 0.85, 0.85, 0.85, 0.85,
  0.85, 0.675, 0.50, 0.325};
//PWM pinout mapping

Serial pc(D1, D0); // tx, rx




DigitalOut AP(D3);
DigitalOut AN(D9);
DigitalOut BP(D8);
DigitalOut BN(D12);
DigitalOut CP(D6);
DigitalOut CN(D7);
FastPWM Boost(A0);


AnalogIn DCCurrentIn (A2);
AnalogIn DCVoltageIn (A5);
//Heartbeat LED
DigitalOut myled(LED1);
Thread thread;
Thread thread2;
Thread thread3;
//Ticker to update the PWM dutycycle
Ticker pwm_ticker;
Ticker pwm_tickerN;
Ticker boost_ticker;

int sine_index = 0;
// Ticker calls this fucntion to update the PWM dutycycle
void pwm_duty_updater() {
  static double counter = 0.0f;
  static int sine_index = 0;
  vector <DigitalOut> toZero;
  vector <DigitalOut> toOne;
  if (sine_duty[sine_index] >= counter)
  {
    if(AP == 0) {
      toZero.push_back(AN);
      toOne.push_back(AP);
    }
  }
  else {
    if(AP == 1) {
      toZero.push_back(AP);
      toOne.push_back(AN);
    }
  }

  if (sine_duty[(sine_index + SINE_STEPS/3)%SINE_STEPS] >= counter)
  {
    if(BP == 0) {
      toZero.push_back(BN);
      toOne.push_back(BP);
    }
  }
  else {
    if(BP == 1) {
      toZero.push_back(BP);
      toOne.push_back(BN);
    }
  }

  if (sine_duty[(sine_index + 2*SINE_STEPS/3)%SINE_STEPS] >= counter)
  {
    if(CP == 0) {
      toZero.push_back(CN);
      toOne.push_back(CP);
    }
  }
  else {
    if(CP == 1) {
      toZero.push_back(CP);
      toOne.push_back(CN);
    }
  }

  for (int i = 0; i < toZero.size(); i++) {
    toZero[i] = 0;
  }
  wait_us(1);
  for (int i = 0; i < toOne.size(); i++) {
    toOne[i] = 1;
  }


  sine_index++;
  counter += TRIANGLE_INCRIMENT;

  if (counter <= -1.0f || counter >= 1.0f) {
    TRIANGLE_INCRIMENT *= -1;
  }
  if (sine_index >= SINE_STEPS) {
    sine_index = 0;
  }
}


double getPanelVoltage(){
  return DCVoltageIn.read()*3.3f*16;
}
double getPanelCurrent() {
  return DCCurrentIn.read()*3.3f*1.6f;

}

double calculateDutyCycle()
{
  /*
  static double old_solar_panel_voltage = 0;
  static double old_solar_panel_current = 0;
  static double dutyCycle = 0.5;
  double deltaB = 0.001;
  double current_pannel_voltage = getPanelVoltage();
  double current_panel_current = getPanelCurrent();

  double panel_power = current_panel_current*current_pannel_voltage;
  double past_power = old_solar_panel_current*old_solar_panel_voltage;

  if (panel_power > past_power) {

    dutyCycle += deltaB;
    return dutyCycle
  }
  if (past_power > panel_power) {
  deltaB *= -1;
  dutyCycle += deltaB;
  return dutyCycle;
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
    sine_duty[i] = sin(i * SINE_STEPS_RAD);  // convert sine (-1.0 .. +1.0) into dutycycle (0.0 .. 1.0)
  }

  Boost.period(1.0f/ (10000));

  // Init the Ticker to call the dutycyle updater at the required interval
  // The update should be at (SINE_STEPS * SINE_OUT_FREQ)

  myled = 1;
  __disable_irq();
  pwm_ticker.attach(&pwm_duty_updater, (1.0f / (float)(10 * TRIANGE_FREQUENCY) - 1e-6f));
  boost_ticker.attach(&boostUpdater, 2);
  __enable_irq();


  while(1){ //infinite loop
    myled = myled^1;
    wait(0.5);


  }

}
