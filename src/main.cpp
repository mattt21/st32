
#include "rtos.h"
#include "FastPWM.h"
#include <string>
#include "bleopcode.h"
#include "Inverter.h"
#include "main.h"
#include <queue>
Serial pc(D1, D0); //tx, rx

DigitalOut out (D14);

Mutex voltage_lock;
Mutex freq_lock;
Mutex ratio_lock;
//double volt_ratio = 23.0f;
//double freq_ratio = 6.0f;

double volt_ratio = 0.0f;
double freq_ratio = 0.0f;

bool isCalibrated = false;
double tempValue;
queue <double> Voltage;
double voltage = 0;
int new_voltage = 0;

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

int new_boost = 0;
double voltage_sum = 0;
double voltage_count = 0;

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
  new_voltage = 1;
}
void setReadVoltage() {
  voltage_sum = voltage_sum + 3.3f*101.0f*DCVoltageIn.read();
  voltage_count = voltage_count + 1;
}
void setBoost() {
  new_boost = 1;
}

void calibrate(){
  Boost.period(1.0f / (150000));
  Boost.write(0.23);
  initInverter();
  sensor_ticker.attach(&readVoltage, 1.0f/500);
  boost_ticker.attach(&setBoost, .5);
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


   // Init the Ticker to call the dutycyle updater at the required interval
   // The update should be at (SINE_STEPS * SINE_OUT_FREQ)

   myled = 0;


   out = 1;
   while(1){ //infinite loop
     while(bluetooth.readable()){
       myled = !myled;
       bleData.push_back(bluetooth.getc());
     }
     //found delimiter, process command now
     if(std::find(bleData.begin(), bleData.end(), '_') != bleData.end()){
       //calculateChecksum(bleData);
       EvalCode(bleData);
       bleData.clear();
     }
     if(isCalibrated && new_boost == 1) {
       sensor_ticker.detach();
       //voltageLock.lock();
       voltage = getBoostVoltage();
       ratio_lock.lock();
       double freq = (freq_ratio/volt_ratio)*voltage/1.41421356237f;
       ratio_lock.unlock();
       pc.printf("voltage: %f frequency: %f\n", voltage, freq);
       if(freq<=1.0f) {
         freq = 1.0f;
       }
       if(freq>=60.0f) {
         freq = 60.0f;
       }
       changeMotorFrequency(freq);
       sensor_ticker.attach(&readVoltage, 1.0f/ 500);
       new_boost = 0;
       myled = myled^1;
       //voltageLock.unlock();
    }
    if (isCalibrated && new_voltage == 1) {
      setReadVoltage();
      new_voltage = 0;
    }
  }
}
