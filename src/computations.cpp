#include "computations.h"

#include<iostream>


// Compares two reads and returns the positions of difference between thoses reads
void analyze(Triplet& read, Output& output, std::vector<std::string>& referenceGenome){

//    0 : truePositives
//    1 : falsePositives
//    2 : falseNegatives

    int tempOutput[3] {0, 0, 0};

    uint s1 = read.original.size();
    uint s2 = read.corrected.size();
    uint s3 = read.reference.size();

    if ( s1 > 0 and s2 > 0 and s3 > 0){

        // In case something was removed during correction or the reference is smaller/bigger...
        if (s1 != s2 or s1 != s3 ){

            ++output.wrongSize;
            ++output.nReadsProcessed;

        } else {

            /* How are values computed:
             * If base is corrected and correction is right (same as reference), true positive
             * If base is corrected but correction is wrong (not the reference), false positive
             * If base is not corrected but original is different from reference, false negative
             */

            for (uint i=0; i < s1; ++i){

                if (read.original[i] != read.reference[i]){

                    if (read.corrected[i] == read.reference[i]){

                        ++tempOutput[0];
                        ++output.truePositives;

                    } else {

                        ++tempOutput[2];
                        ++output.falseNegatives;
                    }

                } else if (read.corrected[i] != read.original[i]) {

                    ++tempOutput[1];
                    ++output.falsePositives;
                }

            }

            ++output.nReadsProcessed;
            ++output.nReadsTotal;

            if (tempOutput[1] == 0 and tempOutput[2] == 0){

                if (tempOutput[0] > 0){

                    ++output.goodCorrection;
                }

            } else {

                if (tempOutput[1] <= tempOutput[2]){

//                    if (findSequenceInReference(read.corrected, referenceGenome)){

//                        ++output.correctedInRef;

//                    };

                } else {

                    ++output.badCorrection;

                }
            }
        }

    } else {

        if (read.original.size() > 0 or read.corrected.size() > 0 or read.reference.size() > 0) ++output.nReadsTotal;
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
