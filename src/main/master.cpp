#include "master.h"

// Useless constructor
Master::Master(std::vector<std::string> settingsVector, bool m) : settings { (uint) stoi(settingsVector[0]), (uint) stoi(settingsVector[1]), settingsVector[2],
settingsVector[3], settingsVector[4], settingsVector[5], settingsVector[6] }
{

    this->mem = m;

    this->nextBatchStart = 0;

    // Loads reference genome in memory
    referenceGenome = generateIndex(settings.referenceGenomeName, 31, settings.nThreads);
}



// Main part of the program, handles multithreading
void Master::evaluation() {

    // Gets original, corrected and reference sequences and stores them in temporary files (assigns one thread to each process)
    if (mem){

        std::cout << "Splitting read files" << std::endl;

        std::thread splitOriginal(splitReadFile, settings.readsFileName, settings.nTempFiles);
        std::thread splitCorrected(splitReadFile, settings.correctedFileName, settings.nTempFiles);
        std::thread splitReference(splitReadFile, settings.referenceFileName, settings.nTempFiles);

        // Synchronizes threads:
        splitOriginal.join();
        splitCorrected.join();
        splitReference.join();

        std::cout << "Splitting done" << std::endl;

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

        loadFiles();

        this->nReads = this->reads.size();

        std::vector<std::thread> threads;
        threads.resize(0);

        std::cout << "Analyzing reads ... " << std::endl;

        for (uint i=0; i<settings.nThreads; ++i) {

            threads.push_back(std::thread(&Master::processBatches, this, i));
        }

        for(auto &t : threads){
             t.join();
        }

        this->reads.clear();
    }

    std::cout << "Cleaning up temp files..." << std::endl;

    cleanupTempFiles();

    std::cout << "Computing gain..." << std::endl;

    computeGain(output);

    std::cout << "Generating output files..." << std::endl;

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



// Function called in a thread that processes batches of reads.
void Master::processBatches(uint i){

    while (true){

        this->protector.lock(); // LOCK

        if(this->nextBatchStart > this->nReads) return;

        std::cout << "Starting new batch in thread " << i << " (Progress : " << 100*this->nextBatchStart / this->nReads << " %)." << std::endl;

        readMap::iterator it = reads.begin();
        readMap::iterator end = reads.begin();

        it = std::next(it, this->nextBatchStart);
        this->nextBatchStart += this->batchSize;

        if (this->nextBatchStart > this->nReads - 2){

            end = std::next(end, this->reads.size()-1);

        } else {

            end = std::next(end, this->nextBatchStart);

        }

        this->protector.unlock(); // UNLOCK

        for (; (it != end); ++it){ // iterator is initialized before, no need to put it in loop declaration.

            Triplet r = it->second;

            analyze(r, this->output, this->referenceGenome);
        }
    }

}




void Master::loadOriginalFile() {

    this->protector.lock(); // LOCK

    std::cout << " - Loading original file " << std::endl;

    this->protector.unlock(); // UNLOCK

    std::ifstream file;

    file.open(this->settings.readsFileName);

    std::string line;
    uint32_t readNumber = 0;
    read r;
    Triplet t;

    while(std::getline(file, line)){

        if (line[0] == '>') {

            readNumber = stoi(line.substr(1, line.length()));

        } else {

            r = seq2bin(line, line.size());

            if ( reads.count(readNumber) > 0 ){

                protector.lock(); // LOCK

                reads[readNumber].original = r;
                reads[readNumber].is_filled.flip(0);

                protector.unlock(); // UNLOCK

            } else {

                reset(t);
                t.original = r;
                t.is_filled.flip(0);

                protector.lock(); // LOCK

                reads[readNumber] = t;

                protector.unlock(); // UNLOCK
            }
        }
    }

    file.close();
}




void Master::loadCorrectedFile(){

    this->protector.lock(); // LOCK

    std::cout << " - Loading corrected file " << std::endl;

    this->protector.unlock(); // UNLOCK

    std::ifstream file;

    file.open(this->settings.correctedFileName);

    std::string line;
    uint32_t readNumber = 0;
    read r;
    Triplet t;

    while(std::getline(file, line)){

        if (line[0] == '>') {

            readNumber = stoi(line.substr(1, line.length()));

        } else {

            r = seq2bin(line, line.size());

            if ( reads.count(readNumber) > 0 ){

                protector.lock(); // LOCK

                reads[readNumber].corrected = r;
                reads[readNumber].is_filled.flip(1);

                protector.unlock(); // UNLOCK

            } else {

                reset(t);
                t.corrected = r;
                t.is_filled.flip(1);

                protector.lock(); // LOCK

                reads[readNumber] = t;

                protector.unlock(); // UNLOCK
            }
        }
    }

    file.close();
}




void Master::loadReferenceFile(){

    this->protector.lock(); // LOCK

    std::cout << " - Loading reference file " << std::endl;

    this->protector.unlock(); // UNLOCK

    std::ifstream file;

    file.open(this->settings.referenceFileName);

    std::string line;
    uint32_t readNumber = 0;
    read r;
    Triplet t;

    while(std::getline(file, line)){

        if (line[0] == '>') {

            readNumber = stoi(line.substr(1, line.length()));

        } else {

            r = seq2bin(line, line.size());

            if ( reads.count(readNumber) > 0 ){

                protector.lock(); // LOCK

                reads[readNumber].reference = r;
                reads[readNumber].is_filled.flip(2);

                protector.unlock(); // UNLOCK

            } else {

                reset(t);
                t.reference = r;
                t.is_filled.flip(2);

                protector.lock(); // LOCK

                reads[readNumber] = t;

                protector.unlock(); // UNLOCK
            }
        }
    }

    file.close();
}




void Master::loadFiles() {

    std::cout << "Loading files " << std::endl;

    std::thread loadOriginal(&Master::loadOriginalFile, this);
    std::thread loadCorrected(&Master::loadCorrectedFile, this);
    std::thread loadReference(&Master::loadReferenceFile, this);

    // Synchronizes threads:
    loadOriginal.join();
    loadCorrected.join();
    loadReference.join();

    std::cout << "All files loaded. Total reads : " << this->reads.size() << std::endl;


}
