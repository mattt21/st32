// Copyright (c)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "bleopcode.h"

const char* GetOpName(opcodetype opcode)
{
    switch (opcode)
    {
    // push value
    case OP_0                      : return "0";
    case OP_1                      : return "1";
    case OP_2                      : return "2";
    case OP_3                      : return "3";
    case OP_4                      : return "4";
    case OP_5                      : return "5";
    case OP_6                      : return "6";
    case OP_7                      : return "7";
    case OP_8                      : return "8";
    case OP_9                      : return "9";
    default                        : return "OP_ERROR";
    }
}

bool EvalCode(char* op){
  //First byte is always an opcode
  opcodetype opcode = (opcodetype)op[0];

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
    default :
    {
      return false;
    }

  }

}
