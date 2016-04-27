#pragma once

#include "binary.h"

#include <unordered_map>
#include <map>

struct Triplet{

    std::bitset<3> is_filled;
    read original;
    read corrected;
    read reference;
};

typedef std::unordered_map<uint, Triplet> readMap;

void reset(Triplet& t);
