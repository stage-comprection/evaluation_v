#include "utils.h"


// Check if read is mapped with reverse complement: bit #5 has to be 'on'
bool is_RC(std::bitset<12> binary){

    if (binary.test(4)) return true;

    return false;
}




// Check if read is mapped: bit #3 has to be 'off'
bool is_mapped(std::bitset<12> binary){

    if (binary.test(2)) return false;

    return true;
}




// Counts reads in a fasta file
uint countReads(std::ifstream& file){

    uint count = 0;
    std::string line;

    while(std::getline(file, line)){

        if (line[0] == '>') ++count;
    }

    resetFileIndex(file);

    return count;
}




// Splits a std::string into a std::vector of std::strings according to a specified delimiter (default: \t)
std::vector<std::string> split(std::string str, const std::string delimiter){

    std::vector<std::string> output;
    size_t pos;

    while ((pos = str.find(delimiter)) != std::string::npos){

        output.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }

    output.push_back(str.substr(0, pos));

    return output;
}




// Sets back a file line index to 0 (getline will start from the beginning of the file again)
void resetFileIndex(std::ifstream& file){

    file.clear();
    file.seekg (0, std::ios::beg);
}




// Generates a conversion table to compute the reverse complement of a DNA sequence
std::vector<char> generateRevCompTable(){

    std::vector<char> table(84);
    table[65] = 'T';
    table[67] = 'G';
    table[71] = 'C';
    table[78] = 'N';
    table[84] = 'A';
    return table;
}




// Computes the reverse complement of a DNA sequence using an accession table
std::string reverseComplement(const std::string& read){

    std::string revComp = "";

    for(int i=(read.size()-1); i>=0; i--){
        revComp += revCompTable[(int)read[i]];
    }

    return revComp;
}
