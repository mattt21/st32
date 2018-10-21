#include "mbed.h"
#include "rtos.h"
#include "FastPWM.h"
#include "PWM.h"
#include <string>
#include <vector>
#include "bleopcode.h"
#include "Inverter.h"



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

int main() {

  bluetooth.baud(9600);
  bluetooth.attach(&bleGetData);

  // Init the duty cycle array

  Boost.period(1.0f/ (10000));

  // Init the Ticker to call the dutycyle updater at the required interval
  // The update should be at (SINE_STEPS * SINE_OUT_FREQ)

  myled = 1;
  __disable_irq();
  boost_ticker.attach(&boostUpdater, 2);
  __enable_irq();


  while(1){ //infinite loop
    myled = myled^1;
    wait(0.5);


  }


}
