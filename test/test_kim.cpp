#include <curves.hpp>
#include <RIPFE/ipfe_kim.hpp>

using IPFE::IntVec;

template <class C>
class KimTest : public ::testing::Test{};

TYPED_TEST_SUITE(KimTest, Curves);

TYPED_TEST(KimTest, DecryptsInnerProductsOnlyWithinBounds){
    const auto msk = IPFE::KIM::setup<TypeParam>(4);
    const auto sk = IPFE::KIM::keygen(msk, IntVec{1, -2, 3, 4});

    EXPECT_EQ(IPFE::KIM::dec(sk, IPFE::KIM::enc(msk, IntVec{5, 6, 7, 8}), 0, 100), 46);
    EXPECT_EQ(IPFE::KIM::dec(sk, IPFE::KIM::enc(msk, IntVec{-4, 5, -6, 0}), -100, 100), -32);
    EXPECT_EQ(IPFE::KIM::dec(sk, IPFE::KIM::enc(msk, IntVec{5, 6, 7, 8}), 0, 45), std::nullopt);
}

TYPED_TEST(KimTest, RejectsVectorsOfTheWrongLength){
    const auto msk = IPFE::KIM::setup<TypeParam>(4);

    EXPECT_THROW((void)IPFE::KIM::keygen(msk, IntVec{1, 2, 3}), rbp::ShapeError);
    EXPECT_THROW((void)IPFE::KIM::enc(msk, IntVec{1, 2, 3, 4, 5}), rbp::ShapeError);
}
