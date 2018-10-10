// Copyright (c)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BLE_OPCODE
#define BLE_OPCODE

enum opcodetype
{
  OP_0 = 0x00,
  OP_1 = 0x01,
  OP_2 = 0x02,
  OP_3 = 0x03,
  OP_4 = 0x04,
  OP_5 = 0x05,
  OP_6 = 0x06,
  OP_7 = 0x07,
  OP_8 = 0x08,
  OP_9 = 0x09,
};

const char* GetOpName(opcodetype opcode);
bool EvalCode(char* op);
#endif
