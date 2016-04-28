#pragma once

#include "BBhash.h"
#include "mapping_utils.h"
#include "indexing.h"


bool findRead(const std::string& read, const std::string& reference);

bool mapRead(const std::string& read, const uint64_t k, hash_index index);
