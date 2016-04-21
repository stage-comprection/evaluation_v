#pragma once

#include "read.h"
#include "output.h"
#include "vector"

void analyze(ReadPair& readPair, Output& output, std::vector<std::string>& referenceGenome);

void computeGain(Output& output);

bool findSequenceInReference(std::string& sequence, std::vector<std::string>& reference);
