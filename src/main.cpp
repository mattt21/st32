
#include "rtos.h"
#include "FastPWM.h"
#include <string>
#include "bleopcode.h"
#include "Inverter.h"
#include "main.h"
#include "mbed.h"
#include <queue>
Serial pc(D1, D0); //tx, rx

DigitalOut out (D14);

double tempValue;
queue <double> Voltage;



/*
  device HM-10
  HM10 - STM32
  GND -> GND
  VCC -> (3.3V)
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
Ticker sensor_ticker;



double getBoostVoltage(){
  //return DCVoltageIn.read()*3.3f*46.0f;
  double voltage = 0;
  double size = (double) Voltage.size();
  while(!Voltage.empty()) {
    voltage += (Voltage.front())/size;
    Voltage.pop();
  }
  return voltage;

}
double getPanelCurrent() {
  return DCCurrentIn.read()*3.3f*10.0f/460.0f;

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


std::vector<char> bleData;

//Ensure data streams are correct
bool validateChecksum(std::vector<char> &bleTempData)
{
    int32_t pos = 0;
    char sum = 0;
    for(int i = 0; i < bleTempData.size() - 1; i++){
      sum += bleTempData[i];
    }
    // return 0 or 1 based on the checksum test
    return (sum == bleTempData[bleTempData.size() - 1]) ? 1 : 0;
}

char calculateChecksum(std::vector<char> &bleTempData)
{
    char sum = 0;
    for(int i = 0; i < bleTempData.size(); i++){
      sum += bleTempData[i];
    }
    // put the checksum into the data stream
    return sum;
}
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
  //Place checksum
  //bluetooth.putc(calculateChecksum(bleTempData));
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
void readVoltage() {
  Voltage.push(3.3f * DCVoltageIn.read()* 101.0f);
}

void boostUpdater()
{
  sensor_ticker.detach();
  static double dutyCycle = calculateDutyCycle();
  double voltage = getBoostVoltage();
  double freq = (6.0f/23.0f)*voltage/1.41421356237f;
  pc.printf("voltage: %f frequency: %f\n", voltage, freq);
  if(freq<=22.0f) {
    freq = 22.0f;
  }
  changeMotorFrequency(freq);
  sensor_ticker.attach(&readVoltage, 1.0f / ((float)600));
  myled = myled^1;

}

int main() {

  pc.attach(&onSerialRx);
  tempValue = 0;

  bluetooth.baud(9600);
  //interrupt is failing to call correctly
  //reverted to reading in main loop
  //bluetooth.attach(&bleGetData);
  bleData.clear();

  // Init the duty cycle array

  Boost.period(1.0f / (100000));
  Boost.write(0.23);

  // Init the Ticker to call the dutycyle updater at the required interval
  // The update should be at (SINE_STEPS * SINE_OUT_FREQ)

  myled = 0;
  initInverter();
  sensor_ticker.attach(&readVoltage, 1.0f/((float)600));
  boost_ticker.attach(&boostUpdater, 2);

  /*
  __disable_irq();
  boost_ticker.attach(&boostUpdater, 2);
  __enable_irq();
  */
  out = 1;
  while(1){ //infinite loop
    if(newData) {
      pc.printf("%f\n", inputs[0]);
      newData = false;
      changeMotorFrequency(inputs[0]);
    }
    while(bluetooth.readable()){
      myled = !myled;
      bleData.push_back(bluetooth.getc());
    }
    //found delimiter, process command now
    if(std::find(bleData.begin(), bleData.end(), '_') != bleData.end()){
      //calculateChecksum(bleData);
      EvalCode(bleData);
      bluetooth.putc(' ');
      bluetooth.putc('O');
      bluetooth.putc('K');
      bleData.clear();
    }

  }

}
