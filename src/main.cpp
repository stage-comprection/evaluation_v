#include "master.h"
#include "settings.h"
#include "file_handling.h"
#include "binary.h"


int main(int argc, char *argv[]) {

//    // Checks number of arguments
//    if(argc < 2){

//        std::cerr << "Usage : ./evaluation_v2 settings_file.ini" << std::endl;

//        return -1;

//    } else {

//        std::string settingsFilePath = argv[1];

//        std::vector<std::string> settingsVector = readSettingsFile(settingsFilePath);

//        Master master(settingsVector);

//        master.evaluation();

//        std::cout << "\n--------------------------------------\n\n";
//        std::cout << "Gain : " << master.output.gain << "\n";
//        std::cout << "True Positives : " << master.output.truePositives << "\n";
//        std::cout << "False Positives : " << master.output.falsePositives << "\n";
//        std::cout << "False Negatives : " << master.output.falseNegatives << "\n";
//        std::cout << "\n--------------------------------------\n\n";
//        std::cout << "Total number of reads : " << master.output.nReadsTotal << "\n";
//        std::cout << "Reads processed : " << master.output.nReadsProcessed << "\n";
//        std::cout << "Reads with length problem : " << master.output.wrongSize << "\n";
//        std::cout << "Reads rightly corrected : " << master.output.goodCorrection << "\n";
//        std::cout << "Reads corrected and found in reference : " << master.output.correctedInRef << "\n";
//        std::cout << "Reads wrongly corrected : " << master.output.badCorrection << "\n\n";
//    }

    char x[5] = {'A', 'T', 'G', 'C', 'G'};

    const uint s = sizeof(x)/sizeof(x[0]);

    typedef std::bitset<2*s> bin;
    bin y;

    seq2bin(x, y, s);

    std::cout << "String to bits: ";
    for (uint i=0; i<sizeof(x)/sizeof(x[0]); ++i) std::cout << x[i];
    std::cout << " = ";

    for (uint i = 0; i < y.size(); ++i){

        std::cout << y[i];
    }

    std::cout<<std::endl;

    char t[5];
    bin r = revComp(y);
    const uint s2 = r.size();
    bin2seq(r, t, s2);

    std::cout << "Reverse complement : ";
    for (uint i=0; i<sizeof(x)/sizeof(x[0]); ++i) std::cout << x[i];
    std::cout << " = ";
    for (uint i=0; i<sizeof(t)/sizeof(t[0]); ++i) std::cout << t[i];
    std::cout<<std::endl;

    return 0;

}
