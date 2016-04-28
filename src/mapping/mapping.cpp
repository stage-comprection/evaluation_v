#include "mapping.h"

bool findRead(const std::string& read, const std::string& reference) {

    for (uint i(0); i<read.size(); ++i) {

        if (read[i] != reference[i]) {

            return false;
        }
    }

    return true;
}





bool mapRead(const std::string& read, const uint64_t k, hash_index& index) {

    minimizer kmerS = seq2intStranded((read.substr(0,k)));
    minimizer kmerRC = rc(kmerS,k);
    minimizer kmer = std::min(kmerRC,kmerS);

    uint i(0);
    std::vector<position> positions;

    do {

        uint64_t hash(index.hashmap.lookup(kmer));

        if (hash != ULLONG_MAX){

            positions = index.posVector[hash];

            for (uint j(0); j < positions.size(); ++j) {

                int possrt(positions[j]-i);

                if (possrt >= 0) {

                    if (findRead(read, index.sequence.substr(possrt, read.size()))) return true;

                }
            }
        }

        if (i + k < read.size()) {

            updateMinimizer(kmerS, read[i+k], k);
            updateMinimizerRC(kmerRC, read[i+k], k);
            kmer = std::min(kmerRC, kmerS);
            ++i;

        } else {

            return false;
        }

    } while(true);


}
