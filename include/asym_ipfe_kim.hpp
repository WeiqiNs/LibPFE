#pragma once

#include "asym_ipfe.hpp"

namespace asym::ipfe::kim {
    // Struct for the public parameters.
    struct Pp {
        int size;
        int bound;
        asym::g1 g1_base;
        asym::g2 g2_base;
        asym::point mod;
    };

    // We define a new struct for the secret key to include determinant value.
    struct Sk {
        asym::Zp det;
        asym::zpMat B;
        asym::zpMat Bi;
    };

    // Struct for the derived functional key.
    struct Key {
        asym::g1Vec ct;
        asym::g1 ctl;
    };

    // Struct for the ciphertext.
    struct Ct {
        asym::g2Vec ct;
        asym::g2 ctr;
    };

    /**
     * Generate public parameters which contains the following.
     *  - size: message length.
     *  - bound: inner product result bound.
     *  - g1_base: a generator in group G1.
     *  - g2_base: a generator in group G2.
     *  - mod: the size of the field.
     * @param size - message length.
     * @param bound - inner product result bound.
     * @return the generated public parameters.
     */
    Pp ppgen(int size, int bound);

    /**
     * Generate the secret key for the scheme.
     * @param pp - the public parameters.
     * @return the generated secret key.
     */
    Sk setup(Pp pp);

    /**
     * Derive a functional key on the input.
     * @param pp - the public parameters.
     * @param sk - the secret key.
     * @param function - the input function vector.
     * @return the functional key.
     */
    Key keyGen(Pp pp, Sk sk, const int *function);

    /**
     * Encrypt a message vector.
     * @param pp - the public parameters.
     * @param sk - the secret key.
     * @param message - the input message vector.
     * @return the ciphertext.
     */
    Ct enc(Pp pp, Sk sk, const int *message);

    /**
     * Compute inner product between a functional key and a ciphertext.
     * @param pp - the public parameters.
     * @param y - a functional key.
     * @param x - a ciphertext.
     * @return the integer result of the inner product.
     */
    int dec(Pp pp, Key y, Ct x);
}