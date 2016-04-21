#pragma once

#include "output.h"

#include <string>


struct Read{

    uint id;
    std::string seq;

    bool operator < (const Read& read) const{
        return(id < read.id);
    }

    bool operator > (const Read& read) const{
        return(id > read.id);
    }
};



struct ReadPair{

    uint id;
    std::string originalSequence;
    std::string correctedSequence;
    std::string referenceSequence;

};
