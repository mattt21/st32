#include "mbed.h"
#include "rtos.h"
#include "FastPWM.h"
#include "PWM.h"
#include <string>
#include <vector>
#include "bleopcode.h"
//#include "Inverter.h"

double TRIANGLE_INCRIMENT = .3;
double  SINE_OUT_FREQ   =  60;

Ticker pwm_ticker;

Serial pc(D1, D0); //tx, rx
DigitalOut AP(D3);
DigitalOut AN(D9);
DigitalOut BP(D8);
DigitalOut BN(D12);
DigitalOut CP(D6);
DigitalOut CN(D7);
DigitalOut out (D14);
// Number of elements in sampled sine wave
#define SINE_STEPS        400
// Frequency of output triangle in Hz
#define TRIANGE_FREQUENCY 500
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
/*
  device HM-10
  HM10 - STM32
  GND -> GND
  VCC -> (5V)
  RX <-> D10 (tx)
  TX <-> D2(rx)
*/
Serial bluetooth(D10, D2); //tx, rx
//Heartbeat LED
DigitalOut myled(LED1);
//PWM pinout mapping

FastPWM Boost(A0);

AnalogIn DCCurrentIn (A2);
AnalogIn DCVoltageIn (A5);


//Ticker to update the PWM dutycycle
Ticker boost_ticker;




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
  return 0.23;
}

void boostUpdater()
{
  static double dutyCycle = calculateDutyCycle();
  Boost.write(dutyCycle);
}

/*
 * Interrupt for receiving
 * data from smart-phone
 */
void bleGetData(){
  std::vector<char> bleData;
  bleData.clear();
  while(bluetooth.readable()){
    myled = !myled;
    bleData.push_back(bluetooth.getc());
    //pc.printf("%c ", bleData[bleData.size() - 1]);
    //bluetooth.putc(bleData[bleData.size() - 1]);
  }
  EvalCode(bleData);
}

void blePushData(std::vector<char> &bleData){
  EvalCode(bleData);
}

void changeMotorFrequency(double freq) {
  int required_freq = freq;
  if(freq>60.0f) {
    required_freq = 60;
  }

  pwm_ticker.detach();
  pwm_ticker.attach(&pwm_duty_updater, 1.0f/(10.0f*40.0f*required_freq));
}

volatile bool newData = false;
volatile float inputs[3];

void onSerialRx() {

  static char serialInBuffer[32];
  static int serialCount = 0;

 while (pc.readable()) { // in case two bytes are ready
   char byteIn = pc.getc();
   if ( (byteIn == 0x0D) || ( (byteIn == 0x0A))) { // if an end of line is found
    serialInBuffer[serialCount] == 0;                 // null terminate the input
    float x;
    if (sscanf(serialInBuffer,"%f",&x) == 1) { // managed to read all 3 values
      inputs[0] = x;
      newData = true;
    }
    serialCount = 0; // reset the buffer
   } else {
    serialInBuffer[serialCount] = byteIn;            // store the character
    if (serialCount<32)                                      // increase the counter.
      serialCount++;
  }
}
}

int main() {

  pc.attach(&onSerialRx);
  bluetooth.baud(9600);
  bluetooth.attach(&bleGetData);

  // Init the duty cycle array

  Boost.period(1.0f/ (100000));
  Boost.write(0.23);

  // Init the Ticker to call the dutycyle updater at the required interval
  // The update should be at (SINE_STEPS * SINE_OUT_FREQ)

  myled = 1;
  initInverter();
  __disable_irq();
  //boost_ticker.attach(&boostUpdater, 2);
  __enable_irq();

  out = 1;
  while(1){ //infinite loop
    if(newData) {
      pc.printf("%f\n", inputs[0]);
      newData = false;
      changeMotorFrequency(inputs[0]);
    }

  }


}
