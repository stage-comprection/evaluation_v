#include "master.h"

// Useless constructor
Master::Master(std::vector<std::string> settingsVector, bool m) : settings { (uint) stoi(settingsVector[0]), (uint) stoi(settingsVector[1]), settingsVector[2],
settingsVector[3], settingsVector[4], settingsVector[5], settingsVector[6] }
{

    this->mem = m;

    this->nextBatchStart = 0;

    // Loads reference genome in memory
//    referenceGenome = generateIndex(settings.referenceGenomeName, 31, settings.nThreads);
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

        std::cout << "Creating threads :" << std::endl;

        for (uint i=0; i<settings.nThreads; ++i) {

            std::cout << " - Thread " << i << " created" << std::endl;
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
        analyze(r, output, referenceGenome, false);
    }

    reads.clear();
}



// Function called in a thread that processes batches of reads.
void Master::processBatches(uint i){

    while (this->nextBatchStart < this->nReads){

        this->protector.lock(); // LOCK

        std::cout << "Starting new batch in thread " << i << ". (Reads processed so far : " << 100*this->nextBatchStart / this->nReads << " %)" << std::endl;
        std::cout << " - Next batch start : " << this->nextBatchStart << " | nReads : " << this->nReads << " | nReadsProcessed " << this->output.nReadsProcessed << " | nReadsTotal " << this->output.nReadsTotal << std::endl;

        this->protector.unlock(); // UNLOCK

        readMap::iterator it = reads.begin();
        readMap::iterator end = reads.begin();

        this->protector.lock(); // LOCK

        it = std::next(it, this->nextBatchStart);
        this->nextBatchStart += this->batchSize;

        std::cout << "NextBatchStart : " << this->nextBatchStart << std::endl;
        std::cout << "nReads : " << this->nReads << std::endl;
        std::cout << "End : " << end->first << std::endl;

        if (this->nextBatchStart > this->nReads - 2){

            std::cout << "1" << std::endl;

            end = this->reads.end();

        } else {

            std::cout << "2" << std::endl;

            end = std::next(end, this->nextBatchStart);

        }

        std::cout << "NextBatchStart : " << this->nextBatchStart << std::endl;
        std::cout << "nReads : " << this->nReads << std::endl;
        std::cout << "End : " << end->first << std::endl;

        this->protector.unlock(); // UNLOCK

        bool print = false;

        for (; (it != end); ++it){ // it is initialized before, no need to put it in loop declaration.

            if (100*this->nextBatchStart / this->nReads > 98){

                print = true;
                std::cout << it->first << std::endl;
            }

            Triplet r = it->second;

            this->protector.lock();
            analyze(r, this->output, this->referenceGenome, print);
            this->protector.unlock();
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
