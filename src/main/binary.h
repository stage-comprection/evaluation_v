#pragma once

#include "utils.h"
#include <bitset>

typedef std::bitset<400> read;

// Convert a sequence of char into binary
read seq2bin(std::string& seq, const uint s);

// Convert a binary sequence into string
std::string bin2seq(read& bin, const uint s);

// RevComp on a bitset
read revComp(read& b);

// Compare two bitsets
void compare(read& b1, read& b2, uint& a, uint& b);
