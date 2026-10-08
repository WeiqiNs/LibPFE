#include <cstddef>
#include <cstdint>
#include <latch>
#include <optional>
#include <thread>
#include <vector>
#include "scheme_types.hpp"

using QFE::IntMat;
using QFE::IntVec;

namespace{
    constexpr std::size_t thread_count = 4;

    template <class Decrypt, class Key, class Ct>
    std::vector<std::optional<std::int64_t>> decrypt_on_threads(
        const Decrypt& decrypt, const Key& key, const std::vector<Ct>& cts
    ){
        std::vector<std::optional<std::int64_t>> results(cts.size());
        std::latch start(thread_count);
        {
            std::vector<std::jthread> threads;
            for (std::size_t t = 0; t < thread_count; ++t){
                threads.emplace_back([&, t]{
                    start.arrive_and_wait();
                    for (auto i = t; i < cts.size(); i += thread_count) results[i] = decrypt(key, cts[i]);
                });
            }
        }
        return results;
    }
}

template <class Scheme>
class ConcurrentInnerProductTest : public ::testing::Test{};

TYPED_TEST_SUITE(ConcurrentInnerProductTest, InnerProductSchemes);

TYPED_TEST(ConcurrentInnerProductTest, ThreadsShareOneKeyAndTable){
    const auto msk = TypeParam::setup(4);
    const auto decrypt = TypeParam::decryptor(msk, -100, 100);
    const auto sk = keygen(msk, IntVec{1, -2, 3, 4});
    const auto prepared = prepare(sk);
    std::vector<decltype(enc(msk, IntVec{}))> cts;
    std::vector<std::optional<std::int64_t>> expected;
    for (std::int64_t k = 0; k < 2 * static_cast<std::int64_t>(thread_count); ++k){
        cts.push_back(enc(msk, IntVec{k, 1, -k, 2}));
        expected.emplace_back(6 - 2 * k);
    }

    EXPECT_EQ(decrypt_on_threads(decrypt, sk, cts), expected);
    EXPECT_EQ(decrypt_on_threads(decrypt, prepared, cts), expected);
}

template <class Scheme>
class ConcurrentQuadraticTest : public ::testing::Test{};

TYPED_TEST_SUITE(ConcurrentQuadraticTest, QuadraticSchemes);

TYPED_TEST(ConcurrentQuadraticTest, ThreadsShareOneKeyAndTable){
    const auto keys = TypeParam::setup(3);
    const auto decrypt = TypeParam::decryptor(keys, -100, 100);
    const auto sk = keygen(keys.msk, IntMat{{1, 0, 2}, {0, -1, 0}, {3, 1, 1}});
    const auto prepared = prepare(sk);
    std::vector<decltype(enc(keys.pk, IntVec{}, IntVec{}))> cts;
    std::vector<std::optional<std::int64_t>> expected;
    for (std::int64_t k = 0; k < 2 * static_cast<std::int64_t>(thread_count); ++k){
        cts.push_back(enc(keys.pk, IntVec{k, 1, 0}, IntVec{1, k, -1}));
        expected.emplace_back(-2 * k);
    }

    EXPECT_EQ(decrypt_on_threads(decrypt, sk, cts), expected);
    EXPECT_EQ(decrypt_on_threads(decrypt, prepared, cts), expected);
}
