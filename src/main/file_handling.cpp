#include "file_handling.h"
#include "binary.h"

// Reads settings from configuration file
std::vector<std::string> readSettingsFile(const std::string& settingsFilePath){

    std::ifstream settingsFile;
    settingsFile.open(settingsFilePath);

    std::string line = "";
    std::vector<std::string> splittedLine, output;
    output.resize(7);

    while(getline(settingsFile, line)){

        splittedLine = split(line, "=");

        if ( splittedLine[0] == "nThreads" ){

            output[0] = splittedLine[1];
        }

        else if ( splittedLine[0] == "nTempFiles" ){

            output[1] = splittedLine[1];
        }

        else if ( splittedLine[0] == "readsFileName" ){

            output[2] = splittedLine[1];
        }

        else if ( splittedLine[0] == "referenceFileName" ){

            output[3] = splittedLine[1];
        }

        else if ( splittedLine[0] == "correctedFileName" ){

            output[4] = splittedLine[1];
        }

        else if ( splittedLine[0] == "outputFileName" ){

            output[5] = splittedLine[1];
        }

        else if ( splittedLine[0] == "referenceGenomeName" ){

            output[6] = splittedLine[1];
        }
    }

    settingsFile.close();

    return output;
}





std::vector<std::string> loadReferenceGenome(const std::string& referenceGenomeName) {

    std::ifstream file;
    file.open(referenceGenomeName);

    std::vector<std::string> referenceGenome;

    std::string line = "", sequence = "";

    while(std::getline(file, line)){

        if (line[0] == '>' and sequence.size() != 0){

            referenceGenome.push_back(sequence);
            sequence = "";

        } else if (line.size() > 0){

            sequence += line;

        }
    }

    referenceGenome.push_back(sequence);

    return referenceGenome;
}





void splitReadFile(const std::string& fileName, uint n) {

    std::vector<std::ofstream> tempFiles;
    std::string temp_f;

    for (uint i=0; i<n; ++i){

        temp_f = "temp_" + fileName + "_" + std::to_string(i);
        std::ofstream ofile;
        ofile.open(temp_f, std::ios::binary);
        tempFiles.push_back(move(ofile));
    }

    std::ifstream file;
    file.open(fileName);

    std::string line;
    read r;
    uint readNumber = 0, fileNumber = 0;

    while(std::getline(file, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1, line.length()));
            fileNumber = readNumber % n;

        } else {

            r = seq2bin(line, line.size());
            tempFiles[fileNumber].write((const char*)&readNumber, sizeof(readNumber));
            tempFiles[fileNumber].write((const char*)&r, sizeof(r));
        }
    }

    for (uint i=0; i<n; ++i) tempFiles[i].close();
}





void cleanupTempFiles() {

    // Using standard libray, a directory and its content are represented by pointers with specific types
    DIR* directory =  opendir("./");
    struct dirent* directoryContent;

    if(!directory) return; // Directory not open

    // Iterates over directory content and remove each file starting with 'temp'
    while ((directoryContent=readdir(directory))){

        std::string fileName = directoryContent->d_name;

        if(fileName.find("temp") != std::string::npos){
          remove(fileName.c_str());
        }
    }

    closedir(directory);
}





void writeOutputFile(Output& output, const std::string& fileName) {

    std::ofstream outputFile;
    outputFile.open(fileName);

    outputFile << "Gain" << "\t" << "TP" << "\t" << "FP" << "\t" << "FN" << "\t" << "SizeDiff" << "\t" << "TotalReads" << "\t" <<
                  "ProcessedReads" << "\t" << "GR" << "\t" << "GC" << "\t" << "RC" << "\t" << "BC" << "\n";
    outputFile << output.gain << "\t" << output.truePositives << "\t" << output.falsePositives << "\t" << output.falseNegatives << "\t"
               << output.wrongSize << "\t" <<output.nReadsTotal << "\t" << output.nReadsProcessed << "\t" << output.goodReads << "\t"
               << output.goodCorrection << "\t" << output.correctedInRef << "\t" << output.badCorrection << "\n";

    outputFile.close();

    std::ofstream outputFile_hr;
    std::string fileName_hr = fileName + "_hr";
    outputFile_hr.open(fileName_hr);

    outputFile_hr << "Gain : " << output.gain << "\n";
    outputFile_hr << "True Positives : " << output.truePositives << "\n";
    outputFile_hr << "False Positives : " << output.falsePositives << "\n";
    outputFile_hr << "False Negatives : " << output.falseNegatives << "\n";
    outputFile_hr << "\n--------------------------------------\n\n";
    outputFile_hr << "Total number of reads : " << output.nReadsTotal << "\n";
    outputFile_hr << "Reads processed : " << output.nReadsProcessed << "\n";
    outputFile_hr << "Reads with length problem : " << output.wrongSize << "\n";
    outputFile_hr << "Reads that did not need correction : " << output.goodReads << "\n";
    outputFile_hr << "Reads rightly corrected : " << output.goodCorrection << "\n";
    outputFile_hr << "Reads corrected and found in reference : " << output.correctedInRef << "\n";
    outputFile_hr << "Reads wrongly corrected : " << output.badCorrection << "\n";

    outputFile_hr.close();

}



void getReadsFromTempFiles(readMap& reads, const uint n, const Settings& settings) {

    std::ifstream original, corrected, reference;

    std::string fileName = "temp_" + settings.readsFileName + "_" + std::to_string(n);
    original.open(fileName, std::ios::binary);

    fileName = "temp_" + settings.correctedFileName + "_" + std::to_string(n);
    corrected.open(fileName, std::ios::binary);

    fileName = "temp_" + settings.referenceFileName + "_" + std::to_string(n);
    reference.open(fileName, std::ios::binary);

    uint32_t readNumber = 0;
    read r;
    Triplet t;

//    std::cout << "  - Loading file 1" << std::endl;

    // Fill ReadPair seq_1.

    original.seekg(0, std::ios::end);
    auto end = (int) original.tellg();
    original.seekg((std::ios::beg));

    while (original.tellg() < end){

        original.read((char*)&readNumber, sizeof(uint));
        original.read((char*)&r, sizeof(read));
        reset(t);
        t.original = r;
        t.is_filled.flip(0);
        reads[readNumber] = t;
    }

    readNumber = 0;

//    std::cout << "  - Loading file 2" << std::endl;

    // Fill seq_2 (direct access to ReadPair using ID, since ID = position in the vector)

    corrected.seekg(0, std::ios::end);
    end = (int) corrected.tellg();
    corrected.seekg((std::ios::beg));

    while (corrected.tellg() < end){

        corrected.read((char*)&readNumber, sizeof(uint));

        if ( reads.count(readNumber) > 0 ){

            corrected.read((char*)&r, sizeof(read));
            reads[readNumber].corrected = r;
            reads[readNumber].is_filled.flip(1);

        } else {

            reset(t);
            t.corrected = r;
            t.is_filled.flip(1);
            reads[readNumber] = t;
        }
    }


//    std::cout << "  - Loading file 3" << std::endl;

    // Fill seq_2 (direct access to ReadPair using ID, since ID = position in the vector)

    reference.seekg(0, std::ios::end);
    end = (int) reference.tellg();
    reference.seekg((std::ios::beg));

    while (reference.tellg() < end){

        reference.read((char*)&readNumber, sizeof(uint));

        if ( reads.count(readNumber) > 0 ){

            reference.read((char*)&r, sizeof(read));
            reads[readNumber].reference = r;
            reads[readNumber].is_filled.flip(2);

        } else {

            reset(t);
            t.reference = r;
            t.is_filled.flip(2);
            reads[readNumber] = t;
        }
    }

    original.close();
    corrected.close();
    reference.close();
}




void loadFiles(readMap& reads, const Settings& settings) {

    std::cout << "Loading Files" << std::endl;

    std::ifstream original, corrected, reference;

    original.open(settings.readsFileName);
    corrected.open(settings.correctedFileName);
    reference.open(settings.referenceFileName);

    std::string line;
    uint32_t readNumber = 0;
    read r;
    Triplet t;

    while(std::getline(original, line)) {

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1, line.length()));

        } else {

            reset(t);
            t.original = r;
            t.is_filled.flip(0);
            reads[readNumber] = t;
        }
    }

    readNumber = 0;

    while(std::getline(corrected, line)){

        if (line[0] == '>') {

            readNumber = stoi(line.substr(1, line.length()));

        } else {

            r = seq2bin(line, line.size());

            if ( reads.count(readNumber) > 0 ){

                reads[readNumber].corrected = r;
                reads[readNumber].is_filled.flip(1);

            } else {

                reset(t);
                t.corrected = r;
                t.is_filled.flip(1);
                reads[readNumber] = t;
            }
        }
    }

    readNumber = 0;

    while(std::getline(reference, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1, line.length()));

        } else {

            r = seq2bin(line, line.size());

            if ( reads.count(readNumber) > 0 ){

                reads[readNumber].reference = r;
                reads[readNumber].is_filled.flip(2);

            } else {

                reset(t);
                t.reference = r;
                t.is_filled.flip(2);
                reads[readNumber] = t;
            }
        }
    }

    original.close();
    corrected.close();
    reference.close();

    std::cout << " - Files loaded" << std::endl;
}
