#pragma once

#include "output.h"
#include "read_storage.h"
#include "../mapping/mapping.h"

#include <vector>
#include <string>
#include <unordered_map>

void analyze(Triplet& read, Output& output, hash_index &index);

void computeGain(Output& output);

bool findSequenceInReference(std::string& sequence, std::vector<std::string>& reference);
