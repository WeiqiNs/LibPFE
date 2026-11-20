#include "scheme_types.hpp"

using QFE::IntMat;
using QFE::IntVec;

template <class Scheme>
class QuadraticTest : public ::testing::Test{};

TYPED_TEST_SUITE(QuadraticTest, QuadraticSchemes);

TYPED_TEST(QuadraticTest, DecryptsQuadraticFormsExactlyWithinTheRange){
    const auto keys = TypeParam::setup(3);
    const auto decrypt = TypeParam::decryptor(keys, -100, 100);
    const auto sk = keygen(keys.msk, IntMat{{1, 0, 2}, {0, -1, 0}, {3, 1, 1}});
    const auto prepared = prepare(sk);
    const std::vector<std::tuple<IntVec, IntVec, std::optional<std::int64_t>>> cases{
        {{1, -2, 3}, {4, 5, -6}, 35}, {{-1, 1, 0}, {2, 0, 1}, -4}, {{0, 0, 0}, {4, 5, -6}, 0},
        {{1, 0, 0}, {100, 0, 0}, 100}, {{0, 1, 0}, {0, 100, 0}, -100}, {{1, 0, 0}, {101, 0, 0}, std::nullopt},
        {{0, 1, 0}, {0, 101, 0}, std::nullopt}
    };

    for (const auto& [left, right, expected] : cases){
        const auto ct = enc(keys.pk, left, right);
        EXPECT_EQ(decrypt(sk, ct), expected);
        EXPECT_EQ(decrypt(prepared, ct), expected);
    }
}

TYPED_TEST(QuadraticTest, DecryptsOneCiphertextUnderManyKeys){
    const auto keys = TypeParam::setup(3);
    const auto decrypt = TypeParam::decryptor(keys, -100, 100);
    const auto ct = enc(keys.pk, IntVec{1, -2, 3}, IntVec{4, 5, -6});

    const std::vector<std::pair<IntMat, std::int64_t>> cases{
        {{{1, 0, 2}, {0, -1, 0}, {3, 1, 1}}, 35}, {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}, -24},
        {{{0, 0, 0}, {0, 0, 0}, {0, 0, 1}}, -18}
    };

    for (const auto& [function, expected] : cases){
        const auto sk = keygen(keys.msk, function);
        EXPECT_EQ(decrypt(sk, ct), expected);
        EXPECT_EQ(decrypt(prepare(sk), ct), expected);
    }
}

TYPED_TEST(QuadraticTest, HandlesVectorsOfLengthOne){
    const auto keys = TypeParam::setup(1);
    const auto decrypt = TypeParam::decryptor(keys, -100, 100);

    EXPECT_EQ(decrypt(keygen(keys.msk, IntMat{{-3}}), enc(keys.pk, IntVec{7}, IntVec{2})), -42);
}

TYPED_TEST(QuadraticTest, RejectsDimensionsBelowOne){
    EXPECT_THROW((void)TypeParam::setup(0), rbp::ShapeError);
}

TYPED_TEST(QuadraticTest, RejectsCiphertextsMissingAPoint){
    const auto keys = TypeParam::setup(3);
    const auto decrypt = TypeParam::decryptor(keys, -100, 100);
    const auto sk = keygen(keys.msk, IntMat{{1, 0, 2}, {0, -1, 0}, {3, 1, 1}});
    auto ct = enc(keys.pk, IntVec{1, -2, 3}, IntVec{4, 5, -6});
    auto& bilinear_g2 = [](auto& c) -> auto&{
        if constexpr (requires{ c.d; }) return c.d;
        else return c.b0;
    }(ct);
    bilinear_g2.pop_back();

    EXPECT_THROW((void)decrypt(sk, ct), rbp::ShapeError);
    EXPECT_THROW((void)decrypt(prepare(sk), ct), rbp::ShapeError);
}

TYPED_TEST(QuadraticTest, RejectsInputsOfTheWrongShape){
    const auto keys = TypeParam::setup(3);

    EXPECT_THROW((void)keygen(keys.msk, IntMat{{1, 2}, {3, 4}, {5, 6}}), rbp::ShapeError);
    EXPECT_THROW((void)keygen(keys.msk, IntMat{{1, 2}, {3, 4}}), rbp::ShapeError);
    EXPECT_THROW((void)keygen(keys.msk, IntMat{{1, 2, 3}, {4, 5, 6}}), rbp::ShapeError);
    EXPECT_THROW((void)enc(keys.pk, IntVec{1, 2}, IntVec{1, 2, 3}), rbp::ShapeError);
    EXPECT_THROW((void)enc(keys.pk, IntVec{1, 2, 3}, IntVec{1, 2, 3, 4}), rbp::ShapeError);
}
