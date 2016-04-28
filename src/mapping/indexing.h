#pragma once

#include "BBhash.h"
#include "mapping_utils.h"

#include <utility>


typedef boomphf::SingleHashFunctor<uint64_t>  hasher;
typedef boomphf::mphf<uint64_t, hasher> MPHF;

struct hash_index{

    std::string sequence;
    MPHF hashmap;
    std::vector<std::vector<position>> posVector;
};

hash_index generateIndex(const std::string& refFileName, uint k, uint core);
