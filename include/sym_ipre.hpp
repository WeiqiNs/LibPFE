#pragma once

#include "sym_vector.hpp"
#include "sym_matrix.hpp"

namespace sym::ipre {
    const int B_SIZE = 4;

    // Struct for secret key.
    struct Sk {
        zpMat A;
        zpMat B;
        zpMat Bi;
        g g_base;
        gt gt_base;
        point mod;
    };

    struct Ct {
        gVec ctx;
        gVec ctk;
        gVec ctc;
    };

    Sk setup(point secpar, int size);

    Ct enc(Sk sk, const int *message, int size);

    int eval(Sk sk, Ct x, Ct y, int size, int bound);
}