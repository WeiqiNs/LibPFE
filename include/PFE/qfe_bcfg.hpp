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
        std::vector<rbp::G1<C>> af;
        std::vector<rbp::G2<C>> fb;
    };

    template <class C>
    struct PreparedSk{
        rbp::Matrix<C> f;
        rbp::G1<C> s1;
        rbp::G1<C> s2;
        std::vector<rbp::G1<C>> af;
        rbp::PreparedG2<C> fb;
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

    namespace detail{
        template <class C, class Key>
        [[nodiscard]] std::optional<std::int64_t> decrypt(
            const rbp::DlogTable<C>& table, const Key& sk, const Ct<C>& ct
        ){
            rbp::PairingProduct<C> product;
            QFE::detail::add_bilinear(product, ct.c, sk.f, ct.d);
            product.add(QFE::detail::negated(sk.af), ct.d_hat);
            product.add(QFE::detail::negated(ct.c_hat), sk.fb);
            product.add(-sk.s1, ct.e);
            product.add(sk.s2, ct.e_hat);
            return table.find(product.evaluate());
        }
    }

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
        const auto fb = f * msk.b;
        auto s1 = rbp::G1<C>::mul_generator(rbp::inner(msk.a, fb) + gamma * msk.w);
        auto af = rbp::G1<C>::mul_generator(msk.a * f);
        return {
            std::move(f), std::move(s1), rbp::G1<C>::mul_generator(gamma), std::move(af),
            rbp::G2<C>::mul_generator(fb)
        };
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
            QFE::detail::masked(pk.a, r, x),
            QFE::detail::masked(pk.a, t, x * s),
            QFE::detail::masked(pk.b, s, y),
            QFE::detail::masked(pk.b, z, y * r),
            rbp::G2<C>::mul_generator(blind),
            pk.w * blind
        };
    }

    template <class C>
    [[nodiscard]] PreparedSk<C> prepare(const Sk<C>& sk){
        return {sk.f, sk.s1, sk.s2, sk.af, rbp::PreparedG2<C>(sk.fb)};
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(const rbp::DlogTable<C>& table, const Sk<C>& sk, const Ct<C>& ct){
        return detail::decrypt(table, sk, ct);
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(
        const rbp::DlogTable<C>& table, const PreparedSk<C>& sk, const Ct<C>& ct
    ){
        return detail::decrypt(table, sk, ct);
    }
}

#endif
