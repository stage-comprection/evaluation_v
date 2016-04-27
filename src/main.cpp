#include "master.h"
#include "settings.h"
#include "file_handling.h"
#include "binary.h"


int main(int argc, char *argv[]) {

    // Checks number of arguments
    if(argc < 2){

        std::cerr << "Usage : ./evaluation_v2 settings_file.ini" << std::endl;

        return -1;

    } else {

        std::string settingsFilePath = argv[1];

        std::vector<std::string> settingsVector = readSettingsFile(settingsFilePath);

        Master master(settingsVector);

        master.evaluation();

        std::cout << "\n--------------------------------------\n\n";
        std::cout << "Gain : " << master.output.gain << "\n";
        std::cout << "True Positives : " << master.output.truePositives << "\n";
        std::cout << "False Positives : " << master.output.falsePositives << "\n";
        std::cout << "False Negatives : " << master.output.falseNegatives << "\n";
        std::cout << "\n--------------------------------------\n\n";
        std::cout << "Total number of reads : " << master.output.nReadsTotal << "\n";
        std::cout << "Reads processed : " << master.output.nReadsProcessed << "\n";
        std::cout << "Reads with length problem : " << master.output.wrongSize << "\n";
        std::cout << "Reads that did not need correction : " << master.output.goodReads << "\n";
        std::cout << "Reads rightly corrected : " << master.output.goodCorrection << "\n";
        std::cout << "Reads corrected and found in reference : " << master.output.correctedInRef << "\n";
        std::cout << "Reads wrongly corrected : " << master.output.badCorrection << "\n\n";
    }

    return 0;

}
