// Copyright (c)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "bleopcode.h"
#include "Inverter.h"
#include "main.h"
#include <string>
#include <sstream>
#include <cstdio>
#include <stdio.h>
#include "mbed.h"

template <typename T>
std::string ToString(T val)
{
    std::stringstream stream;
    stream.precision(4);
    stream << std::fixed << val;
    return stream.str();
}

bool EvalCode(std::vector<char> &bleData){
  //First two bytes always an opcode
  if(bleData.size() < 2)
    return false;

  std::string dataString(bleData.begin(),bleData.end());
  std::size_t found_op = dataString.find("_");
  char opcodeHex[2];
  opcodeHex[0] = bleData[found_op - 2];
  opcodeHex[1] = bleData[found_op - 1];
  int number = (int)strtol(opcodeHex, NULL, 16);
  //Opcode value from 0-31
  //0-15 sends values
  //16-31 receives values
  printf("%d\n", number);
  opcodetype opcode = (opcodetype)number;
  switch(opcode){

    /*
     * 0-15 input data only
     * Use ToString to convert number data to string format
     * vector<char>(str.begin(), str.end()) gets correct number
     */
    //Get voltage
    case OP_0 :
    {
      voltage_lock.lock();
      std::string tempstringValue = ToString(voltage);
      voltage_lock.unlock();
      std::vector<char> tempVec(tempstringValue.begin(), tempstringValue.end());
      tempVec.push_back('0');
      tempVec.push_back('0');
      tempVec.push_back('_');
      blePushData(tempVec);
      return true;
    }
    //Get voltage
    case OP_1 :
    {
      voltage_lock.lock();
      std::string tempstringValue = ToString(voltage);
      voltage_lock.unlock();
      std::vector<char> tempVec(tempstringValue.begin(), tempstringValue.end());
      tempVec.push_back('0');
      tempVec.push_back('1');
      tempVec.push_back('_');
      blePushData(tempVec);
      return true;
    }
    //Get freq
    case OP_2 :
    {
      freq_lock.lock();
      std::string tempstringValue = ToString(frequency);
      freq_lock.unlock();
      std::vector<char> tempVec(tempstringValue.begin(), tempstringValue.end());
      tempVec.push_back('0');
      tempVec.push_back('2');
      tempVec.push_back('_');
      blePushData(tempVec);
      return true;
    }
    case OP_3 :
    {
      std::vector<char> tempVec;
      tempVec.push_back('3');
      blePushData(tempVec);
      return true;
    }
    case OP_4 :
    {
      std::vector<char> tempVec;
      tempVec.push_back('4');
      blePushData(tempVec);
      return true;
    }
    case OP_5 :
    {
      std::vector<char> tempVec;
      tempVec.push_back('5');
      blePushData(tempVec);
      return true;
    }
    case OP_6 :
    {
      return true;
    }
    case OP_7 :
    {
      return true;
    }
    case OP_8 :
    {
      return true;
    }
    case OP_9 :
    {
      return true;
    }
    case OP_10 :
    {
      return true;
    }
    case OP_11 :
    {
      return true;
    }
    case OP_12 :
    {
      return true;
    }
    case OP_13 :
    {
      return true;
    }
    case OP_14 :
    {
      return true;
    }
    case OP_15 :
    {
      return true;
    }

    /*
     * 16-31 output data only
     */
    //Set freq last from app
    case OP_16 :
    {
      if(!isCalibrated){
        calibrate();
      }
      isCalibrated = true;

      //remove opcode and delimiter
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      std::string data(bleData.begin(),bleData.end());
      float token = atof(data.c_str());
      ratio_lock.lock();
      freq_ratio = token/10.0f;
      ratio_lock.unlock();
      printf("token: %llf\n",token);
      return true;
    }
    //Set volt
    case OP_17 :
    {
      //remove opcode and delimiter
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      std::string data(bleData.begin(),bleData.end());
      float token = atof(data.c_str());
      ratio_lock.lock();
      volt_ratio = token/10.0f;
      ratio_lock.unlock();
      printf("token: %llf\n",token);
      return true;
    }
    //Set power
    case OP_18 :
    {
      //remove opcode and delimiter
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      std::string data(bleData.begin(),bleData.end());
      float token = atof(data.c_str());
      //freq_ratio = token/10.0f;
      printf("token: %llf\n",token);
      return true;
    }
    //Set duty
    case OP_19 :
    {
      //remove opcode and delimiter
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      std::string data(bleData.begin(),bleData.end());
      float token = atof(data.c_str());
      double duty_cycle = token/100.0f;
      setDuty(duty_cycle);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_20 :
    {
      //remove opcode and delimiter
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      float token = atof(&bleData[0]);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_21 :
    {
      //remove opcode and delimiter
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      float token = atof(&bleData[0]);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_22 :
    {
      //remove opcode and delimiter
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      float token = atof(&bleData[0]);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_23 :
    {
      //remove opcode and delimiter
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      float token = atof(&bleData[0]);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_24 :
    {
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      float token = atof(&bleData[0]);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_25 :
    {
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      float token = atof(&bleData[0]);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_26 :
    {
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      float token = atof(&bleData[0]);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_27 :
    {
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      float token = atof(&bleData[0]);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_28 :
    {
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      float token = atof(&bleData[0]);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_29 :
    {
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      float token = atof(&bleData[0]);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_30 :
    {
      bleData.pop_back();
      bleData.pop_back();
      bleData.pop_back();
      float token = atof(&bleData[0]);
      printf("%llf\n",token);
      return true;
    }
    //Unused
    case OP_31 :
    {
      std::vector<char> tempVec;
      tempVec.push_back('3');
      tempVec.push_back('1');
      blePushData(tempVec);
      return true;
    }
    //Unused
    default :
    {
      std::vector<char> tempVec;
      tempVec.push_back('?');
      blePushData(tempVec);
      return false;
    }
  }
}
