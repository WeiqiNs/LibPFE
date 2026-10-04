#ifndef RIPFE_IPFE_OPT_HPP
#define RIPFE_IPFE_OPT_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include "ipfe.hpp"

namespace IPFE::OPT{
    inline constexpr std::size_t b_size = 4;

    template <class C>
    struct Msk{
        rbp::Matrix<C> a;
        rbp::Matrix<C> b;
        rbp::Matrix<C> bi;
    };

    template <class C>
    struct Sk{
        std::vector<rbp::G2<C>> r;
        std::vector<rbp::G2<C>> vec;
    };

    template <class C>
    struct Ct{
        std::vector<rbp::G1<C>> r;
        std::vector<rbp::G1<C>> vec;
    };

    template <class C>
    [[nodiscard]] Msk<C> setup(const std::size_t size){
        auto b = rbp::Matrix<C>::random(b_size, b_size);
        auto bi = b.inverse().transpose();
        return {rbp::Matrix<C>::random(2, size), std::move(b), std::move(bi)};
    }

    template <class C>
    [[nodiscard]] rbp::Gt<C> base(){
        return rbp::Gt<C>::generator();
    }

    template <class C>
    [[nodiscard]] Sk<C> keygen(const Msk<C>& msk, const IntVec& function){
        const auto f = to_vector<C>(function);
        const auto s = rbp::random_vector<C>(2);
        const auto fas = msk.a * f + msk.a * msk.a.transpose() * s;
        return {rbp::G2<C>::mul_generator(msk.b * rbp::concat(s, fas)), rbp::G2<C>::mul_generator(s * msk.a + f)};
    }

    template <class C>
    [[nodiscard]] Ct<C> enc(const Msk<C>& msk, const IntVec& message){
        const auto m = to_vector<C>(message);
        const auto s = rbp::random_vector<C>(2);
        return {
            rbp::G1<C>::mul_generator(msk.bi * rbp::concat(msk.a * m, s)),
            rbp::G1<C>::mul_generator(s * msk.a + m)
        };
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(const rbp::DlogTable<C>& table, const Sk<C>& sk, const Ct<C>& ct){
        return table.find(rbp::pair(ct.vec, sk.vec) / rbp::pair(ct.r, sk.r));
    }
}

#endif
