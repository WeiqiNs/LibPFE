#pragma once

#include "asym_vector.hpp"
#include "asym_matrix.hpp"

namespace asym::ipfe {
    const int B_SIZE = 4;

    // Struct for secret key.
    struct Sk {
        zpMat A;
        zpMat B;
        zpMat Bi;
        g1 g1_base;
        g2 g2_base;
        gt gt_base;
        point mod;
    };

    struct Key {
        g1Vec ctx;
        g1Vec ctk;
    };

    struct Ct {
        g2Vec ctx;
        g2Vec ctc;
    };

    Sk setup(point secpar, int size);

    Key keyGen(Sk key, const int *function, int size);

    Ct enc(Sk key, const int *message, int size);

    int dec(Sk key, Key x, Ct y, int size, int bound);
}