#pragma once

#include "file_handling.h"
#include "settings.h"
#include "output.h"
#include "read.h"
#include "computations.h"

#include <thread>


class Master {

    public:

        std::vector<std::string> referenceGenome;

        Master(std::vector<std::string> settingsVector);

        Output output;

        const Settings settings;

        // Loads small read files in memory, compares original/corrected/reference sequences and increments counters accordingly
        void processOneBatch(uint n);

        // Calculate the gain obtained after correction, using the number of True Positives, False Positives and False Negatives
        void evaluation();

};
