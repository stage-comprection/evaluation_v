#pragma once

#include "BBhash.h"
#include "mapping_utils.h"
#include "indexing.h"
#include "../main/read_storage.h"


bool findRead(const std::string& read, const std::string& reference);

bool mapRead(Triplet& t, uint64_t k, hash_index& index);
