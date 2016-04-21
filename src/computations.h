#pragma once

#include "output.h"
#include "readStorage.h"

#include <vector>
#include <string>
#include <unordered_map>

void analyze(Triplet& read, Output& output, std::vector<std::string>& referenceGenome);

void computeGain(Output& output);

bool findSequenceInReference(std::string& sequence, std::vector<std::string>& reference);
