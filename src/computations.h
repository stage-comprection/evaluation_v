#pragma once

#include "output.h"
#include "read_storage.h"

#include <vector>
#include <string>
#include <unordered_map>

void analyze(Triplet& read, Output& output);

void computeGain(Output& output);

bool findSequenceInReference(std::string& sequence, std::vector<std::string>& reference);
