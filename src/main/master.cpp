#include "master.h"

// Useless constructor
Master::Master(std::vector<std::string> settingsVector, bool m) : settings { (uint) stoi(settingsVector[0]), (uint) stoi(settingsVector[1]), settingsVector[2],
settingsVector[3], settingsVector[4], settingsVector[5], settingsVector[6] }
{

    this->mem = m;

    // Loads reference genome in memory
    if (mem) referenceGenome = generateIndex(settings.referenceGenomeName, 31, settings.nThreads);
}



// Main part of the program, handles multithreading
void Master::evaluation() {

    // Gets original, corrected and reference sequences and stores them in temporary files (assigns one thread to each process)
    if (mem){

        std::thread splitOriginal(splitReadFile, settings.readsFileName, settings.nTempFiles);
        std::thread splitCorrected(splitReadFile, settings.correctedFileName, settings.nTempFiles);
        std::thread splitReference(splitReadFile, settings.referenceFileName, settings.nTempFiles);

        // Synchronizes threads:
        splitOriginal.join();
        splitCorrected.join();
        splitReference.join();

        uint processedBatches = 0;
        std::vector<std::thread> threads;

        while (processedBatches < settings.nTempFiles) {

            threads.resize(0);

            for (uint i=0; i<settings.nThreads; ++i) {

                std::cout << "\nProcessing batch : " << processedBatches << " / " << settings.nTempFiles - 1 << std::endl;

                threads.push_back(std::thread(&Master::processOneBatch, this, processedBatches));
                ++processedBatches;
                if (processedBatches == settings.nTempFiles) break;
            }

            for(auto &t : threads){
                 t.join();
            }
        }

    } else {

        readMap reads;
        loadFiles(reads, settings);

        for (auto it=reads.cbegin(); it != reads.cend(); ++it){

            Triplet r = it->second;
            analyze(r, output, referenceGenome);
        }

        reads.clear();
    }

    cleanupTempFiles();

    computeGain(output);

    writeOutputFile(output, settings.outputFileName);
}




// Loads small read files in memory, compares original/corrected/reference sequences and increments counters accordingly
void Master::processOneBatch(uint batchNumber){

    readMap reads;

    getReadsFromTempFiles(reads, batchNumber, settings);

    for (auto it=reads.cbegin(); it != reads.cend(); ++it){

        Triplet r = it->second;
        analyze(r, output, referenceGenome);
    }

    reads.clear();
}
