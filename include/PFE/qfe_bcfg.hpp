#ifndef PFE_QFE_BCFG_HPP
#define PFE_QFE_BCFG_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include "qfe.hpp"

namespace QFE::BCFG{
    template <class C>
    struct Msk{
        rbp::Zp<C> w;
        rbp::Vector<C> a;
        rbp::Vector<C> b;
    };

    template <class C>
    struct Pk{
        std::vector<rbp::G1<C>> a;
        std::vector<rbp::G2<C>> b;
        rbp::G2<C> w;
    };

    template <class C>
    struct Keys{
        Pk<C> pk;
        Msk<C> msk;
    };

    template <class C>
    struct Sk{
        rbp::Matrix<C> f;
        rbp::G1<C> s1;
        rbp::G1<C> s2;
    };

    template <class C>
    struct Ct{
        std::vector<rbp::G1<C>> c;
        std::vector<rbp::G1<C>> c_hat;
        std::vector<rbp::G2<C>> d;
        std::vector<rbp::G2<C>> d_hat;
        rbp::G2<C> e;
        rbp::G2<C> e_hat;
    };

    template <class C>
    [[nodiscard]] Keys<C> setup(const std::size_t size){
        const auto w = rbp::Zp<C>::random();
        auto a = rbp::random_vector<C>(size);
        auto b = rbp::random_vector<C>(size);
        return {
            {rbp::G1<C>::mul_generator(a), rbp::G2<C>::mul_generator(b), rbp::G2<C>::mul_generator(w)},
            {w, std::move(a), std::move(b)}
        };
    }

    template <class C>
    [[nodiscard]] rbp::Gt<C> base(){
        return rbp::Gt<C>::generator();
    }

    template <class C>
    [[nodiscard]] Sk<C> keygen(const Msk<C>& msk, const IntMat& function){
        auto f = to_matrix<C>(function);
        const auto gamma = rbp::Zp<C>::random();
        auto s1 = rbp::G1<C>::mul_generator(rbp::inner(msk.a, f * msk.b) + gamma * msk.w);
        return {std::move(f), std::move(s1), rbp::G1<C>::mul_generator(gamma)};
    }

    template <class C>
    [[nodiscard]] Ct<C> enc(const Pk<C>& pk, const IntVec& left, const IntVec& right){
        const auto x = to_vector<C>(left);
        const auto y = to_vector<C>(right);
        const auto r = rbp::Zp<C>::random();
        const auto s = rbp::Zp<C>::random();
        const auto t = rbp::Zp<C>::random();
        const auto z = rbp::Zp<C>::random();
        const auto blind = r * s - z - t;
        return {
            detail::masked(pk.a, r, x),
            detail::masked(pk.a, t, x * s),
            detail::masked(pk.b, s, y),
            detail::masked(pk.b, z, y * r),
            rbp::G2<C>::mul_generator(blind),
            pk.w * blind
        };
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(
        const rbp::DlogTable<C>& table, const Pk<C>& pk, const Sk<C>& sk, const Ct<C>& ct
    ){
        return table.find(
            detail::bilinear(ct.c, sk.f, ct.d) / detail::bilinear(pk.a, sk.f, ct.d_hat)
            / detail::bilinear(ct.c_hat, sk.f, pk.b) / rbp::pair(sk.s1, ct.e) * rbp::pair(sk.s2, ct.e_hat)
        );
    }
}

#endif
