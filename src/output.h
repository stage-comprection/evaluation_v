#pragma once

#include<atomic>

struct Output{

    std::atomic<uint64_t> truePositives{0};
    std::atomic<uint64_t> falsePositives{0};
    std::atomic<uint64_t> falseNegatives{0};
    std::atomic<uint64_t> wrongSize{0};
    std::atomic<uint64_t> nReadsProcessed{0};
    std::atomic<uint64_t> nReadsTotal{0};
    std::atomic<uint64_t> goodCorrection{0};
    std::atomic<uint64_t> correctedInRef{0};
    std::atomic<uint64_t> badCorrection{0};
    float gain{0};

};
