#include "binary.h"

// Convert a char into a nucleotide
nuc char2nuc(char c){

    return char2nuc_map[c];
}


// Convert a nucleotide into a char
char nuc2char(nuc n){

    return nuc2char_map[n];
}

// Functions converting a sequence of nuc into char and a sequence of char into nuc
void seq2bin(char* seq, nuc* binSeq){


}

void bin2seq(nuc* binSeq, char* seq);

// Reverse complement
void revComp(nuc* seq, nuc* r_seq);

