#pragma once

#include "BBhash.h"
#include "mapping_utils.h"
#include "indexing.h"


bool findRead(const std::string& read, const std::string& reference);

bool mapRead(const std::string& read,const uint64_t k, const std::string& ref, hash_index index);
