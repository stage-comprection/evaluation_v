#include "master.h"
#include "settings.h"
#include "file_handling.h"


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

        std::cout << std::endl << " >> Gain = " << master.output.gain << std::endl;
    }

    return 0;

}
