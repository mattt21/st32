// Copyright (c)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MAIN_H
#define MAIN_H
#include <vector>
#include <mbed.h>
#include <algorithm>

extern double tempValue;

void blePushData(std::vector<char> &bleData);

extern Mutex freq_lock;

#endif
