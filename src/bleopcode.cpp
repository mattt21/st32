// Copyright (c)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "bleopcode.h"

bool EvalCode(std::vector<char> &bleData){
  //First two bytes always an opcode
  if(bleData.size() < 2)
    return false;

  opcodetype opcode = (opcodetype)(bleData[0] + bleData[1]);

  switch(opcode){

    case OP_0 :
    {
      return true;
    }
    case OP_1 :
    {
      return true;
    }
    case OP_2 :
    {
      return true;
    }
    case OP_3 :
    {
      return true;
    }
    case OP_4 :
    {
      return true;
    }
    case OP_5 :
    {
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
    default :
    {
      return false;
    }

  }

}
