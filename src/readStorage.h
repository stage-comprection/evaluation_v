#pragma once

#include "binary.h"

#include <unordered_map>

struct Triplet{

    read original;
    read corrected;
    read reference;
};

typedef std::unordered_map<uint, Triplet> readMap;
