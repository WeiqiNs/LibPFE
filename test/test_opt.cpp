#include <curves.hpp>
#include <RIPFE/ipfe_opt.hpp>

using IPFE::IntVec;

template <class C>
class OptTest : public ::testing::Test{};

TYPED_TEST_SUITE(OptTest, Curves);

TYPED_TEST(OptTest, OneTableDecryptsInnerProductsOnlyWithinItsBounds){
    const auto msk = IPFE::OPT::setup<TypeParam>(4);
    const rbp::DlogTable<TypeParam> table(IPFE::OPT::base<TypeParam>(), -100, 100);
    const auto sk = IPFE::OPT::keygen(msk, IntVec{1, -2, 3, 4});

    EXPECT_EQ(IPFE::OPT::dec(table, sk, IPFE::OPT::enc(msk, IntVec{5, 6, 7, 8})), 46);
    EXPECT_EQ(IPFE::OPT::dec(table, sk, IPFE::OPT::enc(msk, IntVec{-4, 5, -6, 0})), -32);
    EXPECT_EQ(IPFE::OPT::dec(table, sk, IPFE::OPT::enc(msk, IntVec{5, 6, 7, 30})), std::nullopt);
}

TYPED_TEST(OptTest, RejectsVectorsOfTheWrongLength){
    const auto msk = IPFE::OPT::setup<TypeParam>(4);

    EXPECT_THROW((void)IPFE::OPT::keygen(msk, IntVec{1, 2, 3}), rbp::ShapeError);
    EXPECT_THROW((void)IPFE::OPT::enc(msk, IntVec{1, 2, 3, 4, 5}), rbp::ShapeError);
}
