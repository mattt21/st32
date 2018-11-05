
#include "rtos.h"
#include "FastPWM.h"
#include "PWM.h"
#include <string>
#include "bleopcode.h"
#include "Inverter.h"
#include "main.h"
#include "mbed.h"

double tempValue;

/*
  device HM-10
  HM10 - STM32
  GND -> GND
  VCC -> (5V)
  RX <-> D10 (tx)
  TX <-> D2(rx)
*/
Serial bluetooth(D10, D2); //tx, rx
Serial pc(D1, D0); //tx, rx
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
  return 0.2;
}

void boostUpdater()
{
  static double dutyCycle = calculateDutyCycle();
  Boost.write(dutyCycle);
}
std::vector<char> bleData;
/*
 * Interrupt for receiving
 * data from smart-phone
 * '_' triggers eoc(end-of-command)
 * opcode pulled from last 2 values
 */
void bleGetData(){
  while(bluetooth.readable()){
    myled = !myled;
    bleData.push_back(bluetooth.getc());
  }
  //found delimiter, process command now
  if(std::find(bleData.begin(), bleData.end(), '_') != bleData.end()){
    //EvalCode(bleData);
    bluetooth.putc(' ');
    bluetooth.putc('O');
    bluetooth.putc('K');
    bleData.clear();
  }
}

void blePushData(std::vector<char> &bleTempData){
  for(int i = 0; i < bleTempData.size(); i++){
    bluetooth.putc(bleTempData[i]);
  }
}

int main() {

  tempValue = 0;
  bluetooth.baud(9600);
  //interrupt is failing to call correctly
  //reverted to reading in main loop
  //bluetooth.attach(&bleGetData);
  bleData.clear();

  // Init the duty cycle array

  Boost.period(1.0f/ (10000));

  // Init the Ticker to call the dutycyle updater at the required interval
  // The update should be at (SINE_STEPS * SINE_OUT_FREQ)

  myled = 0;
  initInverter();
  __disable_irq();
  boost_ticker.attach(&boostUpdater, 2);
  __enable_irq();

  while(1){ //infinite loop
    //wait(0.5);
    while(bluetooth.readable()){
      myled = !myled;
      bleData.push_back(bluetooth.getc());
    }
    //found delimiter, process command now
    if(std::find(bleData.begin(), bleData.end(), '_') != bleData.end()){
      EvalCode(bleData);
      bluetooth.putc(' ');
      bluetooth.putc('O');
      bluetooth.putc('K');
      bleData.clear();
    }
  }
  
}
