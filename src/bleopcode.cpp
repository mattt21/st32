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

    }
    case OP_1 :
    {

    }
    case OP_2 :
    {

    }
    case OP_3 :
    {

    }
    case OP_4 :
    {

    }
    case OP_5 :
    {

    }
    case OP_6 :
    {

    }
    case OP_7 :
    {

    }
    case OP_8 :
    {

    }
    case OP_9 :
    {

    }
    case OP_10 :
    {

    }
    default :
    {
      return false;
    }

  }

}
