// Copyright (c)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "bleopcode.h"
#include <string>
#include <sstream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

template <typename T>
std::string ToString(T val)
{
    std::stringstream stream;
    stream << val;
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
  opcodetype opcode = (opcodetype)number;
  float num = 1.121f;
  std::string tempstringValue = ToString(num);

  switch(opcode){

    /*
     * 0-15 input data only
     * Use ToString to convert number data to string format
     * vector<char>(str.begin(), str.end()) gets correct number
     */
    case OP_0 :
    {
      std::vector<char> tempVec(tempstringValue.begin(), tempstringValue.end());
      tempVec.push_back('0');
      tempVec.push_back('0');
      tempVec.push_back('_');
      blePushData(tempVec);
      return true;
    }
    case OP_1 :
    {
      std::vector<char> tempVec;
      tempVec.push_back('1');
      blePushData(tempVec);
      return true;
    }
    case OP_2 :
    {
      std::vector<char> tempVec;
      tempVec.push_back('2');
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
    case OP_16 :
    {
      return true;
    }
    case OP_17 :
    {
      return true;
    }
    case OP_18 :
    {
      return true;
    }
    case OP_19 :
    {
      return true;
    }
    case OP_20 :
    {
      return true;
    }
    case OP_21 :
    {
      return true;
    }
    case OP_22 :
    {
      return true;
    }
    case OP_23 :
    {
      return true;
    }
    case OP_24 :
    {
      return true;
    }
    case OP_25 :
    {
      return true;
    }
    case OP_26 :
    {
      return true;
    }
    case OP_27 :
    {
      return true;
    }
    case OP_28 :
    {
      return true;
    }
    case OP_29 :
    {
      return true;
    }
    case OP_30 :
    {
      return true;
    }
    case OP_31 :
    {
      std::vector<char> tempVec;
      tempVec.push_back('3');
      tempVec.push_back('1');
      blePushData(tempVec);
      return true;
    }
    default :
    {
      std::vector<char> tempVec;
      tempVec.push_back('?');
      blePushData(tempVec);
      return false;
    }
  }
}
