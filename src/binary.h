#pragma once

#include <utility>
#include <iostream>

typedef unsigned int uint;

// Convert a sequence of char into binary
template <typename T>
void seq2bin(char* seq, T& bin, const uint s){

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
}


// Convert a binary sequence into char
template <typename T>
void bin2seq(T& bin, char* seq, const uint s){

    for (uint i=0; i<s; i+=2){

        switch(bin[i]){
            case 1:
                switch(bin[i+1]){
                    case 1:
                        seq[i/2] = 'T';
                        break;
                    default:
                        seq[i/2] = 'G';
                        break;
                }
                break;
            default:
                switch(bin[i+1]){
                    case 1:
                        seq[i/2] = 'C';
                        break;
                    default:
                        seq[i/2] = 'A';
                        break;
                }
                break;
        }
    }
}


template <typename T>
T revComp(T& b){

    T r;
    uint s = b.size()-1;

    for (int i=s; i>=0; --i){

        r.set(s-i, !b[i]);
    }

    return r;
}
