#pragma once

#include "output.h"
#include "../mapping/mapping.h"

#include <vector>
#include <string>
#include <unordered_map>

void analyze(Triplet& t, Output& output, hash_index &index, bool print);

void computeGain(Output& output);

bool findSequenceInReference(std::string& sequence, std::vector<std::string>& reference);
