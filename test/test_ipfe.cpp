#include "scheme_types.hpp"

using IPFE::IntVec;

template <class Scheme>
class InnerProductTest : public ::testing::Test{};

TYPED_TEST_SUITE(InnerProductTest, InnerProductSchemes);

TYPED_TEST(InnerProductTest, DecryptsInnerProductsExactlyWithinTheRange){
    const auto msk = TypeParam::setup(4);
    const auto decrypt = TypeParam::decryptor(msk, -100, 100);
    const auto sk = keygen(msk, IntVec{1, -2, 3, 4});

    EXPECT_EQ(decrypt(sk, enc(msk, IntVec{5, 6, 7, 8})), 46);
    EXPECT_EQ(decrypt(sk, enc(msk, IntVec{-4, 5, -6, 0})), -32);
    EXPECT_EQ(decrypt(sk, enc(msk, IntVec{2, 1, 0, 0})), 0);
    EXPECT_EQ(decrypt(sk, enc(msk, IntVec{0, 0, 0, 25})), 100);
    EXPECT_EQ(decrypt(sk, enc(msk, IntVec{0, 0, 0, -25})), -100);
    EXPECT_EQ(decrypt(sk, enc(msk, IntVec{1, 0, 0, 25})), std::nullopt);
    EXPECT_EQ(decrypt(sk, enc(msk, IntVec{-1, 0, 0, -25})), std::nullopt);
}

TYPED_TEST(InnerProductTest, DecryptsOneCiphertextUnderManyKeys){
    const auto msk = TypeParam::setup(4);
    const auto decrypt = TypeParam::decryptor(msk, -100, 100);
    const auto ct = enc(msk, IntVec{5, 6, 7, 8});

    EXPECT_EQ(decrypt(keygen(msk, IntVec{1, -2, 3, 4}), ct), 46);
    EXPECT_EQ(decrypt(keygen(msk, IntVec{0, 0, 0, 1}), ct), 8);
    EXPECT_EQ(decrypt(keygen(msk, IntVec{1, 1, 1, 1}), ct), 26);
}

TYPED_TEST(InnerProductTest, PreparedKeyDecryptsEveryCiphertextLikeTheKey){
    const auto msk = TypeParam::setup(4);
    const auto decrypt = TypeParam::decryptor(msk, -100, 100);
    const auto sk = keygen(msk, IntVec{1, -2, 3, 4});
    const auto prepared = prepare(sk);
    const std::vector<std::pair<IntVec, std::optional<std::int64_t>>> cases{
        {{5, 6, 7, 8}, 46}, {{-4, 5, -6, 0}, -32}, {{0, 0, 0, 25}, 100}, {{1, 0, 0, 25}, std::nullopt}
    };

    for (const auto& [message, expected] : cases){
        const auto ct = enc(msk, message);
        EXPECT_EQ(decrypt(prepared, ct), expected);
        EXPECT_EQ(decrypt(sk, ct), expected);
    }
}

TYPED_TEST(InnerProductTest, HandlesVectorsOfLengthOne){
    const auto msk = TypeParam::setup(1);
    const auto decrypt = TypeParam::decryptor(msk, -100, 100);

    EXPECT_EQ(decrypt(keygen(msk, IntVec{-3}), enc(msk, IntVec{7})), -21);
    EXPECT_EQ(decrypt(prepare(keygen(msk, IntVec{-3})), enc(msk, IntVec{7})), -21);
}

TYPED_TEST(InnerProductTest, RejectsVectorsOfTheWrongLength){
    const auto msk = TypeParam::setup(4);

    EXPECT_THROW((void)keygen(msk, IntVec{1, 2, 3}), rbp::ShapeError);
    EXPECT_THROW((void)keygen(msk, IntVec{1, 2, 3, 4, 5}), rbp::ShapeError);
    EXPECT_THROW((void)enc(msk, IntVec{1, 2, 3}), rbp::ShapeError);
    EXPECT_THROW((void)enc(msk, IntVec{1, 2, 3, 4, 5}), rbp::ShapeError);
}
