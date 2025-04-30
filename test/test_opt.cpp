#include <gtest/gtest.h>
#include "ipfe_opt.hpp"

TEST(OptSchemeTests, InBound){
    // Generate the master secret key.
    const auto msk = IPFE::OPT::setup(10);
    // One could assign the base to another variable (treat it as public parameter).
    const auto base = msk.bpg->Gp->get_gt();

    // Set the testing integer vectors.
    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Generate ciphertext and function key.
    const auto sk = IPFE::OPT::keygen(msk, x);
    const auto ct = IPFE::OPT::enc(msk, y);

    // Compute the result.
    const auto r = IPFE::OPT::dec(base, sk, ct, 300, 400);

    // Perform testing.
    EXPECT_EQ(r, 330);

    // Close the group.
    BP::close();
}

TEST(OptSchemeTests, NotInBound){
    // Generate the master secret key.
    const auto msk = IPFE::OPT::setup(10);
    // One could assign the base to another variable (treat it as public parameter).
    const auto base = msk.bpg->Gp->get_gt();

    // Set the testing integer vectors.
    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};

    // Generate ciphertext and function key.
    const auto sk = IPFE::OPT::keygen(msk, x);
    const auto ct = IPFE::OPT::enc(msk, y);

    // Compute the result.
    const auto r = IPFE::OPT::dec(base, sk, ct, 100, 200);

    // Perform testing.
    EXPECT_EQ(r, -1);

    // Close the group.
    BP::close();
}
