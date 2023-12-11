#pragma once

#include "sym_vector.hpp"
#include "sym_matrix.hpp"

namespace sym::ipre {
    // Struct for the public parameters.
    struct Pp {
        bool pre;
        int size;
        int bound;
        sym::g g_base;
        sym::gt gt_base;
        sym::point mod;
        sym::gVec table;
    };

    // Struct for the secret key.
    struct Sk {
        sym::zpMat A;
        sym::zpMat B;
        sym::zpMat Bi;
    };

    // Struct for the ciphertext.
    struct Ct {
        sym::gVec ctx;
        sym::gVec ctl;
        sym::gVec ctr;
    };

    /**
     * Generate public parameters which contains the following.
     *  - pre: a boolean value to indicate whether fast mul in G is used.
     *  - size: message length.
     *  - bound: inner product result bound.
     *  - g_base: a generator in group G.
     *  - gt_base: the result of e(g_base, g_base).
     *  - mod: the size of the field.
     *  - table: a precomputed table, which maybe empty.
     * @param size - message length.
     * @param bound - inner product result bound.
     * @return the generated public parameters.
     */
    Pp ppgen(bool pre, int size, int bound);

    /**
     * Generate the secret key for the scheme.
     * @param pp - the public parameters.
     * @return the generated secret key.
     */
    Sk setup(Pp pp);

    /**
     * Encrypt a message vector.
     * @param pp - the public parameters.
     * @param sk - the secret key.
     * @param message - the input message vector.
     * @return the ciphertext.
     */
    Ct enc(Pp pp, Sk sk, const int *message);

    /**
     * Compute inner product of two messages using their ciphertexts.
     * @param pp - the public parameters.
     * @param x - a ciphertext.
     * @param y - a ciphertext.
     * @return the integer result of the inner product.
     */
    int eval(Pp pp, Ct x, Ct y);
}