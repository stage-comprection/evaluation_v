#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <bitset>

#define LOGOK fprintf(stderr, "ERR : %s:%i\n", __FILE__, __LINE__);

// Check if read is mapped with reverse complement: bit #5 (from the right end) has to be 'on'
bool is_RC(std::bitset<12> binary);

// Check if read is mapped: bit #3 (from the right end) has to be 'on'
bool is_mapped(std::bitset<12> binary);

// Counts reads in a fasta file
uint countReads(std::ifstream& file);

// Generates a conversion table to compute the reverse complement of a DNA sequence
std::vector<char> generateRevCompTable();

// Creates the table for reverse complement to be used as a global variable
const std::vector<char> revCompTable = generateRevCompTable();

// Splits a string into a vector of strings according to a specified delimiter (default: \t)
std::vector<std::string> split(std::string str, const std::string delimiter="\t");

// Sets back a file line index to 0 (getline will start from the beginning of the file again)
void resetFileIndex(std::ifstream& file);

// Computes the reverse complement of a DNA sequence using an accession table
std::string reverseComplement(const std::string& read);

// Find the max ID for reads in a file
uint find_max(std::ifstream& file);
