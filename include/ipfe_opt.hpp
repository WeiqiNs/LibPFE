#pragma once

#include "RBP/bp.hpp"

namespace IPFE::OPT{
    // Struct for master secret key.
    struct Msk{
        FpMat a;
        FpMat b;
        FpMat bi;
        std::unique_ptr<BP> bpg;
    };

    // Struct for function key.
    struct Sk{
        G2Vec r;
        G2Vec vec;
    };

    // Struct for ciphertext.
    struct Ct{
        G1Vec r;
        G1Vec vec;
    };

    /**
     * Perform setup to generate master secret key.
     * @param size message length.
     * @param pre a boolean indicating whether using precompute tables for group exponentiation.
     * @return the generated public parameters.
     */
    Msk setup(int size, const bool& pre = true);

    /**
     * Derive a functional key on the input.
     * @param msk the master secret key.
     * @param function the input function vector.
     * @return the functional key.
     */
    Sk keygen(const Msk& msk, const IntVec& function);

    /**
     * Encrypt a message vector.
     * @param msk the master secret key.
     * @param message the input message vector.
     * @return the ciphertext.
     */
    Ct enc(const Msk& msk, const IntVec& message);

    /**
     * Compute inner product between a functional key and a ciphertext.
     * @param base the base to raise to and compare with.
     * @param sk a functional key.
     * @param ct a ciphertext.
     * @param lower_bound inner product result lower bound.
     * @param upper_bound inner product result upper bound.
     * @return the integer result of the inner product.
     */
    int dec(const Gt& base, const Sk& sk, const Ct& ct, int lower_bound, int upper_bound);
}
