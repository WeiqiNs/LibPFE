#include "scheme_types.hpp"

using IPFE::IntVec;

template <class Scheme>
class InnerProductTest : public ::testing::Test{};

TYPED_TEST_SUITE(InnerProductTest, InnerProductSchemes);

TYPED_TEST(InnerProductTest, DecryptsInnerProductsExactlyWithinTheRange){
    const auto msk = TypeParam::setup(4);
    const auto decrypt = TypeParam::decryptor(msk, -100, 100);
    const auto sk = keygen(msk, IntVec{1, -2, 3, 4});
    const auto prepared = prepare(sk);
    const std::vector<std::pair<IntVec, std::optional<std::int64_t>>> cases{
        {{5, 6, 7, 8}, 46}, {{-4, 5, -6, 0}, -32}, {{2, 1, 0, 0}, 0}, {{0, 0, 0, 25}, 100}, {{0, 0, 0, -25}, -100},
        {{1, 0, 0, 25}, std::nullopt}, {{-1, 0, 0, -25}, std::nullopt}
    };

    for (const auto& [message, expected] : cases){
        const auto ct = enc(msk, message);
        EXPECT_EQ(decrypt(sk, ct), expected);
        EXPECT_EQ(decrypt(prepared, ct), expected);
    }
}

TYPED_TEST(InnerProductTest, DecryptsOneCiphertextUnderManyKeys){
    const auto msk = TypeParam::setup(4);
    const auto decrypt = TypeParam::decryptor(msk, -100, 100);
    const auto ct = enc(msk, IntVec{5, 6, 7, 8});

    EXPECT_EQ(decrypt(keygen(msk, IntVec{1, -2, 3, 4}), ct), 46);
    EXPECT_EQ(decrypt(keygen(msk, IntVec{0, 0, 0, 1}), ct), 8);
    EXPECT_EQ(decrypt(keygen(msk, IntVec{1, 1, 1, 1}), ct), 26);
}

TYPED_TEST(InnerProductTest, HandlesVectorsOfLengthOne){
    const auto msk = TypeParam::setup(1);
    const auto decrypt = TypeParam::decryptor(msk, -100, 100);

    EXPECT_EQ(decrypt(keygen(msk, IntVec{-3}), enc(msk, IntVec{7})), -21);
}

TYPED_TEST(InnerProductTest, RejectsDimensionsBelowOne){
    EXPECT_THROW((void)TypeParam::setup(0), rbp::ShapeError);
}

TYPED_TEST(InnerProductTest, RejectsVectorsOfTheWrongLength){
    const auto msk = TypeParam::setup(4);

    EXPECT_THROW((void)keygen(msk, IntVec{1, 2, 3}), rbp::ShapeError);
    EXPECT_THROW((void)keygen(msk, IntVec{1, 2, 3, 4, 5}), rbp::ShapeError);
    EXPECT_THROW((void)enc(msk, IntVec{1, 2, 3}), rbp::ShapeError);
    EXPECT_THROW((void)enc(msk, IntVec{1, 2, 3, 4, 5}), rbp::ShapeError);
}

TYPED_TEST(InnerProductTest, RejectsCiphertextsOfAnotherDimension){
    const auto msk = TypeParam::setup(4);
    const auto decrypt = TypeParam::decryptor(msk, -100, 100);
    const auto sk = keygen(msk, IntVec{1, -2, 3, 4});
    const auto ct = enc(TypeParam::setup(3), IntVec{1, 2, 3});

    EXPECT_THROW((void)decrypt(sk, ct), rbp::ShapeError);
    EXPECT_THROW((void)decrypt(prepare(sk), ct), rbp::ShapeError);
}
