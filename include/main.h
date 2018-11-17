// Copyright (c)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MAIN_H
#define MAIN_H
#include <vector>
#include <algorithm>
#include "rtos.h"

extern double tempValue;
extern double voltage;
void blePushData(std::vector<char> &bleData);
extern Mutex freq_lock;
extern Mutex voltage_lock;

#endif
