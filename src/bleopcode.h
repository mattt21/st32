// Copyright (c)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BLE_OPCODE
#define BLE_OPCODE

#include <vector>
/*
 * Opcodes are split between
 * input and output.
 * Used to decipher data types sent
 * to each device.
 */
enum opcodetype
{

  /*
   * Reserved for output requests
   */
  OP_0    = 0x00,
  OP_1    = 0x01,
  OP_2    = 0x02,
  OP_3    = 0x03,
  OP_4    = 0x04,
  OP_5    = 0x05,
  OP_6    = 0x06,
  OP_7    = 0x07,
  OP_8    = 0x08,
  OP_9    = 0x09,
  OP_10   = 0x0A,
  OP_11   = 0x0B,
  OP_12   = 0x0C,
  OP_13   = 0x0D,
  OP_14   = 0x0E,
  OP_15   = 0x0F,

  /*
   * Reserved for input requests
   */

   OP_16   = 0x10,
   OP_17   = 0x11,
   OP_18   = 0x12,
   OP_19   = 0x13,
   OP_20   = 0x14,
   OP_21   = 0x15,
   OP_22   = 0x16,
   OP_23   = 0x17,
   OP_24   = 0x18,
   OP_25   = 0x19,
   OP_26   = 0x1A,
   OP_27   = 0x1B,
   OP_28   = 0x1C,
   OP_29   = 0x1D,
   OP_30   = 0x1E,
   OP_31   = 0x0F,

};

bool EvalCode(std::vector<char> &bleData);

#endif
