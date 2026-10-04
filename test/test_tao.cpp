#include <curves.hpp>
#include <RIPFE/ipfe_tao.hpp>

using IPFE::IntVec;

template <class C>
class TaoTest : public ::testing::Test{};

TYPED_TEST_SUITE(TaoTest, Curves);

TYPED_TEST(TaoTest, OneTableDecryptsInnerProductsOnlyWithinItsBounds){
    const auto msk = IPFE::TAO::setup<TypeParam>(4);
    const rbp::DlogTable<TypeParam> table(msk.base, -100, 100);
    const auto sk = IPFE::TAO::keygen(msk, IntVec{1, -2, 3, 4});

    EXPECT_EQ(IPFE::TAO::dec(table, sk, IPFE::TAO::enc(msk, IntVec{5, 6, 7, 8})), 46);
    EXPECT_EQ(IPFE::TAO::dec(table, sk, IPFE::TAO::enc(msk, IntVec{-4, 5, -6, 0})), -32);
    EXPECT_EQ(IPFE::TAO::dec(table, sk, IPFE::TAO::enc(msk, IntVec{5, 6, 7, 30})), std::nullopt);
}

TYPED_TEST(TaoTest, RejectsVectorsOfTheWrongLength){
    const auto msk = IPFE::TAO::setup<TypeParam>(4);

    EXPECT_THROW((void)IPFE::TAO::keygen(msk, IntVec{1, 2, 3}), rbp::ShapeError);
    EXPECT_THROW((void)IPFE::TAO::enc(msk, IntVec{1, 2, 3, 4, 5}), rbp::ShapeError);
}
