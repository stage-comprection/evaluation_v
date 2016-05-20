#include "computations.h"

#include<iostream>


// Compares two reads and returns the positions of difference between thoses reads
void analyze(Triplet& t, Output& output, hash_index& index){

//    0 : truePositives
//    1 : falsePositives
//    2 : falseNegatives

    int tempOutput[3] {0, 0, 0};

    uint s1 = t.original.size();

    if (t.is_filled.all()) {

        /* How are values computed:
         * If base is corrected and correction is right (same as reference), true positive
         * If base is corrected but correction is wrong (not the reference), false positive
         * If base is not corrected but original is different from reference, false negative
         */

        for (uint i=0; i < s1; i+=2){

            if (t.original[i] != t.reference[i] or t.original[i+1] != t.reference[i+1]){

                if (t.corrected[i] == t.reference[i] and t.corrected[i+1] == t.reference[i+1]){

                    ++tempOutput[0];
                    ++output.truePositives;

                } else {

                    ++tempOutput[2];
                    ++output.falseNegatives;
                }

            } else if (t.corrected[i] != t.original[i] or t.corrected[i+1] != t.original[i+1]) {

                ++tempOutput[1];
                ++output.falsePositives;

            } else {

            }
        }

        ++output.nReadsProcessed;
        ++output.nReadsTotal;

        if (tempOutput[1] == 0 and tempOutput[2] == 0){

            if (tempOutput[0] > 0){

                ++output.goodCorrection;

            } else {

                ++output.goodReads;

            }

        } else {

            if (tempOutput[1] <= tempOutput[2]){

                    if (mapRead(t, 31, index)){

                        ++output.correctedInRef;
                        output.falseNegatives -= tempOutput[2];
                        output.falsePositives -= tempOutput[1];
                        output.truePositives += tempOutput[2];
                        output.truePositives += tempOutput[1];

                    };

            } else {

                ++output.badCorrection;

            }
        }


    } else {

        if (t.is_filled.any()) {

            ++output.nReadsTotal;

        } else {


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
