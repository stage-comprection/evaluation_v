#include "master.h"

// Useless constructor
Master::Master(std::vector<std::string> settingsVector) : settings { stoi(settingsVector[0]), stoi(settingsVector[1]), settingsVector[2],
settingsVector[3], settingsVector[4], settingsVector[5], settingsVector[6] }
{
    // Loads reference genome in memory
    referenceGenome = loadReferenceGenome(settings.referenceGenomeName);
}



// Main part of the program, handles multithreading
void Master::evaluation() {

//    // Gets original, corrected and reference sequences and stores them in temporary files (assigns one thread to each process)
//    std::thread splitOriginal(splitReadFile, settings.readsFileName, settings.nTempFiles);
//    std::thread splitCorrected(splitReadFile, settings.correctedFileName, settings.nTempFiles);
//    std::thread splitReference(splitReadFile, settings.referenceFileName, settings.nTempFiles);

//    // Synchronizes threads:
//    splitOriginal.join();
//    splitCorrected.join();
//    splitReference.join();

    uint processedBatches = 0;
    std::vector<std::thread> threads;

    while (processedBatches < settings.nTempFiles) {

        threads.resize(0);

        for (uint i=0; i<settings.nThreads; ++i) {

            std::cout << "\nProcessing batch : " << processedBatches << " / " << settings.nTempFiles << std::endl;

            threads.push_back(std::thread(&Master::processOneBatch, this, processedBatches));
            ++processedBatches;
            if (processedBatches > settings.nTempFiles) break;
        }

        for(auto &t : threads){
             t.join();
        }
    }

//    cleanupTempFiles();

    computeGain(output);

    writeOutputFile(output, settings.outputFileName);

}




// Loads small read files in memory, compares original/corrected/reference sequences and increments counters accordingly
void Master::processOneBatch(uint batchNumber){

    readMap reads;

    getReadsFromTempFiles(reads, batchNumber, settings);

    uint count = 0, nreads = reads.size();
    uint step = (uint) nreads/100;
    uint stepCount = 0;

    std::cout << " - ";

    for (readMap::iterator it = reads.begin(); it != reads.end(); ++it){

        analyze(it->second, output, referenceGenome);

        if (count % step == 0){

            std::cout << "\r  - Analyzing : " << stepCount << " % completed (" << nreads << " reads total).";
            ++stepCount;
        }

        ++count;
    }

    std::cout << "\r  - Analyzing : 100 % completed (" << nreads << " reads total).";
}
