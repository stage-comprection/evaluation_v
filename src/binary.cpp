#include "binary.h"

// Convert a sequence of char into binary
read seq2bin(std::string& seq, const uint s){

    read bin;

    for (uint i=0; i < s; ++i){

        switch(seq[i]){

            case 'T':
                bin.flip(2*i);
                bin.flip(2*i+1);
                break;

            case 'C':
                bin.flip(2*i);
                break;

            case 'G':
                bin.flip(2*i+1);
                break;

            default:
                break;
        }
    }

    return bin;
}




// Convert a binary sequence into string
std::string bin2seq(read& bin, const uint s){

    std::string seq(s/2, 'x');

    for (uint i=0; i<s; i+=2){

        switch((uint)bin[i]){
            case 1:
                switch((uint)bin[i+1]){
                    case 1:
                        seq[i/2] = 'T';
                        break;
                    default:
                        seq[i/2] = 'C';
                        break;
                }
                break;
            default:
                switch((uint)bin[i+1]){
                    case 1:
                        seq[i/2] = 'G';
                        break;
                    default:
                        seq[i/2] = 'A';
                        break;
                }
                break;
        }
    }

    return seq;
}



// RevComp on a bitset
read revComp(read& b){

    read r;
    uint s = b.size()-1;

    for (int i=s; i>=0; --i){

        r.set(s-i, !b[i]);
    }

    return r;
}





