#include "file_handling.h"

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
        ofile.open(temp_f);
        tempFiles.push_back(move(ofile));

    }

    std::ifstream file;
    file.open(fileName);

    std::string line;
    uint readNumber = 0, fileNumber = 0;

    while(std::getline(file, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1, line.length()));
            fileNumber = readNumber % n;
            tempFiles[fileNumber] << line << "\n";

        } else {

            tempFiles[fileNumber] << line << "\n";
        }
    }
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
                  "ProcessedReads" << "GC" << "\t" << "RC" << "\t" << "BC" << "\n";
    outputFile << output.gain << "\t" << output.truePositives << "\t" << output.falsePositives << "\t" << output.falseNegatives << "\t"
               << output.wrongSize << "\t" <<output.nReadsTotal << "\t" << output.nReadsProcessed << "\t" << output.goodCorrection << "\t"
               << output.correctedInRef << "\t" << output.badCorrection << "\n";

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
    outputFile_hr << "Reads rightly corrected : " << output.goodCorrection << "\n";
    outputFile_hr << "Reads corrected and found in reference : " << output.correctedInRef << "\n";
    outputFile_hr << "Reads wrongly corrected : " << output.badCorrection << "\n";

    outputFile_hr.close();

}



void getReadsFromTempFiles(readMap& reads, const uint n, const Settings& settings) {

    std::ifstream original, corrected, reference;

    std::string fileName = "temp_" + settings.readsFileName + "_" + std::to_string(n);
    original.open(fileName);

    fileName = "temp_" + settings.correctedFileName + "_" + std::to_string(n);
    corrected.open(fileName);

    fileName = "temp_" + settings.referenceFileName + "_" + std::to_string(n);
    reference.open(fileName);

    std::string line;
    uint readNumber = 0;

    std::cout << "  - Loading file 1" << std::endl;

    // Fill ReadPair seq_1.
    while(std::getline(original, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1));

        } else {

            reads[readNumber] = Triplet{line, "", ""};
        }
    }

    readNumber = 0;

    std::cout << "  - Loading file 2" << std::endl;

    // Fill seq_2 (direct access to ReadPair using ID, since ID = position in the vector)
    while(std::getline(corrected, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1));

        } else {

            if ( reads.count(readNumber) > 0 ){

                reads[readNumber].corrected = line;

            } else {

                reads[readNumber] = Triplet{"", line, ""};

            }


        }
    }

    std::cout << "  - Loading file 3" << std::endl;

    // Fill seq_2 (direct access to ReadPair using ID, since ID = position in the vector)
    while(std::getline(reference, line)){

        if (line[0] == '>'){

            readNumber = stoi(line.substr(1));

        } else {

            if ( reads.count(readNumber) > 0){

                reads[readNumber].reference = line;

            } else {

                reads[readNumber] = Triplet{"", "", line};

            }
        }
    }

    original.close();
    corrected.close();
    reference.close();
}
