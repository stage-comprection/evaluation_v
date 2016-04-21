#pragma once
#include <bitset>
#include <unordered_map>

// Bitset of length 2 to store a nucleotide
typedef std::bitset<2> nuc;

// Hash maps used for conversion between char and nuc
std::unordered_map<char, nuc> char2nuc_map = {{'A', nuc (0)}, {'T', nuc (3)}, {'C', nuc (2)}, {'G', nuc (1)}};
std::unordered_map<nuc, char> nuc2char_map = {{nuc (0), 'A'}, {nuc (3), 'T'}, {nuc (2), 'T'}, {nuc (1), 'G'}};

// Functions converting char into nuc and nuc into char
nuc char2nuc(char c);
char nuc2char(nuc n);

// Functions converting a sequence of nuc into char and a sequence of char into nuc
void seq2bin(char* seq, nuc* binSeq);
void bin2seq(nuc* binSeq, char* seq);

// Reverse complement
void revComp(nuc* seq, nuc* r_seq);
