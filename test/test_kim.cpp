#include <gtest/gtest.h>
#include "ipfe_kim.hpp"

TEST(KimSchemeTests, InBound){
    // Generate the master secret key.
    const auto msk = IPFE::KIM::setup(10);

    // Set the testing integer vectors.
    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Generate ciphertext and function key.
    const auto sk = IPFE::KIM::keygen(msk, x);
    const auto ct = IPFE::KIM::enc(msk, y);

    // Compute the result.
    const auto r = IPFE::KIM::dec(sk, ct, 300, 400);

    // Perform testing.
    EXPECT_EQ(r, 330);

    // Close the group.
    BP::close();
}

TEST(KimSchemeTests, NotInBound){
    // Generate the master secret key.
    const auto msk = IPFE::KIM::setup(10);

    // Set the testing integer vectors.
    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};

    // Generate ciphertext and function key.
    const auto sk = IPFE::KIM::keygen(msk, x);
    const auto ct = IPFE::KIM::enc(msk, y);

    // Compute the result.
    const auto r = IPFE::KIM::dec(sk, ct, 100, 200);

    // Perform testing.
    EXPECT_EQ(r, -1);

    // Close the group.
    BP::close();
}
