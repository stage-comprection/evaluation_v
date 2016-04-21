#pragma once

#include <string>
#include <unordered_map>

struct Triplet{

    std::string original;
    std::string corrected;
    std::string reference;
};

typedef std::unordered_map<uint, Triplet> readMap;
