#pragma once

#include "file_handling.h"
#include "settings.h"
#include "output.h"
#include "computations.h"
#include "read_storage.h"
#include "../mapping/mapping.h"

#include <mutex>
#include <thread>


class Master {

    public:

        hash_index referenceGenome;

        Output output;

        const Settings settings;

        uint batchSize = 1000000;

        uint64_t nextBatchStart;

        std::mutex protector;

        uint nReads;

        bool mem;

        readMap reads;

        Master(std::vector<std::string> settingsVector, bool m);        

        // Loads small read files in memory, compares original/corrected/reference sequences and increments counters accordingly
        void processOneBatch(uint n);

        void processBatches(uint i);

        void loadOriginalFile();
        void loadCorrectedFile();
        void loadReferenceFile();
        void loadFiles();

        // Calculate the gain obtained after correction, using the number of True Positives, False Positives and False Negatives
        void evaluation();

};
