#include "computations.h"

#include<iostream>


// Compares two reads and returns the positions of difference between thoses reads
void analyze(ReadPair& readPair, Output& output, std::vector<std::string>& referenceGenome){

//    0 : truePositives
//    1 : falsePositives
//    2 : falseNegatives
//    3 : wrongSize
//    4 : nReadsProcessed

    int tempOutput[3] {0, 0, 0};

    // In case something was removed during correction or the reference is smaller/bigger...
    if (readPair.originalSequence.size() != readPair.correctedSequence.size() or
        readPair.originalSequence.size() != readPair.referenceSequence.size() ){

        ++output.wrongSize;
        ++output.nReadsProcessed;

    } else if (readPair.originalSequence != "" and readPair.referenceSequence != "" and readPair.correctedSequence != ""){

        /* How are values computed:
         * If base is corrected and correction is right (same as reference), true positive
         * If base is corrected but correction is wrong (not the reference), false positive
         * If base is not corrected but original is different from reference, false negative
         */

        for (uint i=0; i<readPair.originalSequence.size(); ++i){

            if (readPair.originalSequence[i] != readPair.referenceSequence[i]){

                if (readPair.correctedSequence[i] == readPair.referenceSequence[i]){

                    ++tempOutput[0];
                    ++output.truePositives;

                } else {

                    ++tempOutput[2];
                    ++output.falseNegatives;
                }

            } else if (readPair.correctedSequence[i] != readPair.originalSequence[i]) {

                ++tempOutput[1];
                ++output.falsePositives;
            }
        }

        ++output.nReadsProcessed;

    }

    if (tempOutput[1] == 0 and tempOutput[2] == 0){

        if (tempOutput[0] > 0){

            ++output.goodCorrection;
        }

    } else {

        if (tempOutput[1] <= tempOutput[2]){

                if (findSequenceInReference(readPair.correctedSequence, referenceGenome)){

                    ++output.correctedInRef;

                };

        } else {

            ++output.badCorrection;

        }
    }

}





bool findSequenceInReference(std::string& sequence, std::vector<std::string>& reference){

    for (uint i=0; i<reference.size(); ++i){

        if(reference[i].find(sequence) != std::string::npos) return true;
    }

    return false;
}





void computeGain(Output& output){

    output.gain = ((float) output.truePositives - (float) output.falsePositives) / ((float) output.truePositives + (float) output.falseNegatives);

}
