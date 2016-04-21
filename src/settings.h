#pragma once

#include<string>

struct Settings {

    const uint nThreads;
    const uint nTempFiles;
    const std::string readsFileName;
    const std::string referenceFileName;
    const std::string correctedFileName;
    const std::string outputFileName;
    const std::string referenceGenomeName;

};
