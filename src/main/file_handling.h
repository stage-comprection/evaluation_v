#pragma once

#include "output.h"
#include "utils.h"
#include "settings.h"
#include "read_storage.h"

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <dirent.h>

std::vector<std::string> readSettingsFile(const std::string& settingsFilePath);

std::vector<std::string> loadReferenceGenome(const std::string& referenceGenomePath);

void splitReadFile(const std::string& fileName, uint n);

void cleanupTempFiles();

void writeOutputFile(Output& output, const std::string& fileName);

void getReadsFromTempFiles(readMap& reads, const uint n, const Settings& settings);
