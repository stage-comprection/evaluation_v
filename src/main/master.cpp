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

        std::cout << "Loading files " << std::endl;

        loadFiles();

        std::cout << "Files loaded" << std::endl;

        this->nReads = this->reads.size();

        std::vector<std::thread> threads;
        threads.resize(0);

        for (uint i=0; i<settings.nThreads; ++i) {

            threads.push_back(std::thread(&Master::processBatches, this));
        }

        for(auto &t : threads){
             t.join();
        }

        this->reads.clear();
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



// Function called in a thread that processes batches of reads.
void Master::processBatches(){

    while (this->nextBatchStart < this->nReads){


        this->nextBatchStartProtector.lock();
        std::cout << "Starting new batch (Reads processed : " << this->nextBatchStart / this->nReads << " %)" << std::endl;

        readMap::const_iterator it = this->reads.cbegin();
        readMap::const_iterator end = this->reads.cbegin();
        std::next(it, this->nextBatchStart);
        std::next(end, this->nextBatchStart + this->batchSize);
        this->nextBatchStart += this->batchSize;

        this->nextBatchStartProtector.unlock();

        for (; (it != end and it != this->reads.cend()); ++it){

            Triplet r = it->second;
            analyze(r, this->output, this->referenceGenome);
        }

    }
}




void Master::loadOriginalFile() {

    std::cout << "Loading original file " << std::endl;

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

                readsMapProtector.lock();

                reads[readNumber].original = r;
                reads[readNumber].is_filled.flip(0);

                readsMapProtector.unlock();

            } else {

                reset(t);
                t.original = r;
                t.is_filled.flip(0);

                readsMapProtector.lock();
                reads[readNumber] = t;
                readsMapProtector.unlock();
            }
        }
    }

    file.close();
}




void Master::loadCorrectedFile(){

    std::cout << "Loading corrected file " << std::endl;
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

                readsMapProtector.lock();

                reads[readNumber].corrected = r;
                reads[readNumber].is_filled.flip(1);

                readsMapProtector.unlock();

            } else {

                reset(t);
                t.corrected = r;
                t.is_filled.flip(1);

                readsMapProtector.lock();
                reads[readNumber] = t;
                readsMapProtector.unlock();
            }
        }
    }

    file.close();
}




void Master::loadReferenceFile(){

    std::cout << "Loading reference file " << std::endl;
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

                readsMapProtector.lock();

                reads[readNumber].reference = r;
                reads[readNumber].is_filled.flip(2);

                readsMapProtector.unlock();

            } else {

                reset(t);
                t.reference = r;
                t.is_filled.flip(2);

                readsMapProtector.lock();
                reads[readNumber] = t;
                readsMapProtector.unlock();
            }
        }
    }

    file.close();
}




void Master::loadFiles() {

    std::thread loadOriginal(&Master::loadOriginalFile, this);
    std::thread loadCorrected(&Master::loadCorrectedFile, this);
    std::thread loadReference(&Master::loadReferenceFile, this);

    // Synchronizes threads:
    loadOriginal.join();
    loadCorrected.join();
    loadReference.join();

}
