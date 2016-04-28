#include "indexing.h"

hash_index generateIndex(const std::string& refFileName, uint k, uint coreNumber) {

    std::cout << "Indexing reference ..." << std::endl;

    std::ifstream refFile;
    refFile.open(refFileName);
    std::string seq;
    std::getline(refFile, seq);
    std::getline(refFile, seq);

    MPHF kmer2Indice;
    std::vector<std::vector<position>> indice2Positions;

    std::vector<uint64_t> kmerIndexed;
    uint64_t i(0);

    minimizer kmerS(seq2intStranded((seq.substr(0, k))));
    minimizer kmerRC(rc(kmerS, k));
    minimizer kmer(std::min(kmerRC, kmerS));

    bool end(false);

    do {

        kmerIndexed.push_back(kmer);

        if (seq[i+k] == ':') {

            i += k;

            do {
                ++i;
            } while(seq[i] == ':');

            kmerS = seq2intStranded(seq.substr(i, k));
            kmerRC = rc(kmerS, k);
            kmer = std::min(kmerRC, kmerS);

        } else if (i+k < seq.size()) {

            updateMinimizer(kmerS, seq[i+k], k);
            updateMinimizerRC(kmerRC, seq[i+k], k);
            kmer = std::min(kmerRC, kmerS);
            ++i;

        } else {

            end=true;
        }

    } while(!end);

    std::cout << "Kmers indexed, starting hashing ..." << std::endl;

    auto data_iterator = boomphf::range(static_cast<const uint64_t*>(&((kmerIndexed)[0])), static_cast<const uint64_t*>(&((kmerIndexed)[0]))+kmerIndexed.size());
    kmer2Indice = boomphf::mphf<uint64_t, hasher>(kmerIndexed.size(), data_iterator, coreNumber, 10, false);
    indice2Positions.resize(kmerIndexed.size());
    kmerIndexed.clear();

    i = 0;
    kmerS = seq2intStranded((seq.substr(0, k)));
    kmerRC = rc(kmerS, k);
    kmer = std::min(kmerRC, kmerS);
    end = false;

    do {

        uint64_t hash(kmer2Indice.lookup(kmer));
        indice2Positions[hash].push_back(i);

        if (seq[i+k] == ':') {

            i += k;

            do {
                ++i;
            } while (seq[i] == ':');

            kmerS = seq2intStranded((seq.substr(i, k)));
            kmerRC = rc(kmerS, k);
            kmer = std::min(kmerRC, kmerS);

        } else if (i + k < seq.size()){

            updateMinimizer(kmerS, seq[i+k], k);
            updateMinimizerRC(kmerRC, seq[i+k], k);
            kmer = std::min(kmerRC, kmerS);
            ++i;

        } else {

            end = true;
        }

    } while(!end);

    std::cout << "Indexing done" << std::endl;

    return hash_index {seq, kmer2Indice, indice2Positions};

}
