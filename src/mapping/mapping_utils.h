#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cctype>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <ctime>



typedef uint64_t minimizer;
typedef uint32_t position;
typedef uint64_t kmer;


std::vector<std::string> getReads(std::ifstream& ReadFile,uint64_t n);
uint64_t nuc2int(char c);
std::string reversecomplement (const std::string& s);
void int2seq(minimizer min, uint64_t n);
minimizer cat(minimizer seed, minimizer body, uint64_t n);
minimizer getRepresent(minimizer min, uint64_t n);
minimizer rc(minimizer min, uint64_t n);
minimizer getEnd(minimizer kmer, uint64_t n);
minimizer getBegin(minimizer kmer, uint64_t n);
double jaccardSet(std::unordered_set<minimizer>& set1,std::unordered_set<minimizer>& set2);
uint64_t inANotInB(std::unordered_set<minimizer>& set1,std::unordered_set<minimizer>& set2);
uint64_t interSet(std::unordered_set<minimizer>& set1,std::unordered_set<minimizer>& set2);
std::string getRead(std::ifstream& readFile);
std::unordered_map<minimizer,uint8_t> kmerCounting(const std::string& seq, const uint k);
void readCounting(const std::string& readFile);
void updateMinimizerRC(minimizer&	min, char nuc, uint64_t k);
void updateMinimizer(minimizer&	min, char nuc, uint64_t k);
minimizer seq2intStranded(const std::string& seq);
std::vector<std::string> getReads(std::ifstream& readFile,uint n,uint& readNumber);
