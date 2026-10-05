#ifndef PFE_IPFE_KKS_HPP
#define PFE_IPFE_KKS_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include "ipfe.hpp"

namespace IPFE::KKS{
    template <class C>
    struct Msk{
        rbp::Zp<C> eta;
        rbp::Zp<C> eta_bar;
        rbp::Vector<C> s;
        rbp::Vector<C> t;
        rbp::Vector<C> u;
        rbp::Vector<C> v;
        rbp::Vector<C> h;
        rbp::Vector<C> h_hat;
        rbp::Vector<C> h_bar;
    };

    template <class C>
    struct Sk{
        std::vector<rbp::G2<C>> vec;
    };

    template <class C>
    struct Ct{
        std::vector<rbp::G1<C>> vec;
    };

    template <class C>
    struct PreparedSk{
        rbp::PreparedG2<C> vec;
    };

    namespace detail{
        template <class C>
        [[nodiscard]] rbp::Vector<C> ciphertext_half(
            const Msk<C>& msk, const rbp::Vector<C>& h, const rbp::Vector<C>& m
        ){
            const auto r = rbp::Zp<C>::random();
            const auto ct1 = rbp::concat(rbp::Vector<C>{r, msk.eta * r}, m + h * r);
            return rbp::concat(rbp::Vector<C>{-rbp::inner(msk.u, ct1), -rbp::inner(msk.v, ct1)}, ct1);
        }

        template <class C>
        [[nodiscard]] rbp::Vector<C> key_half(const Msk<C>& msk, const rbp::Vector<C>& key){
            const auto r = rbp::Zp<C>::random();
            return rbp::concat(rbp::Vector<C>{r, msk.eta_bar * r}, key + msk.h_bar * r);
        }
    }

    template <class C>
    [[nodiscard]] Msk<C> setup(const std::size_t size){
        const auto eta = rbp::Zp<C>::random();
        const auto eta_bar = rbp::Zp<C>::random();
        auto s = rbp::random_vector<C>(size);
        auto t = rbp::random_vector<C>(size);
        auto u = rbp::random_vector<C>(size + 2);
        auto v = rbp::random_vector<C>(size + 2);
        auto h = s + t * eta;
        auto h_hat = rbp::random_vector<C>(size) + rbp::random_vector<C>(size) * eta;
        auto h_bar = u + v * eta_bar;
        return {
            eta, eta_bar, std::move(s), std::move(t), std::move(u), std::move(v), std::move(h), std::move(h_hat),
            std::move(h_bar)
        };
    }

    template <class C>
    [[nodiscard]] rbp::Gt<C> base(){
        return rbp::Gt<C>::generator();
    }

    template <class C>
    [[nodiscard]] Sk<C> keygen(const Msk<C>& msk, const IntVec& function){
        const auto f = to_vector<C>(function);
        const auto key = rbp::concat(rbp::Vector<C>{-rbp::inner(msk.s, f), -rbp::inner(msk.t, f)}, f);
        return {
            rbp::G2<C>::mul_generator(
                rbp::concat(detail::key_half(msk, key), detail::key_half(msk, rbp::Vector<C>(key.size())))
            )
        };
    }

    template <class C>
    [[nodiscard]] Ct<C> enc(const Msk<C>& msk, const IntVec& message){
        const auto m = to_vector<C>(message);
        return {
            rbp::G1<C>::mul_generator(
                rbp::concat(detail::ciphertext_half(msk, msk.h, m), detail::ciphertext_half(msk, msk.h_hat, m))
            )
        };
    }

    template <class C>
    [[nodiscard]] PreparedSk<C> prepare(const Sk<C>& sk){
        return {rbp::PreparedG2<C>(sk.vec)};
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(const rbp::DlogTable<C>& table, const Sk<C>& sk, const Ct<C>& ct){
        return table.find(rbp::pair(ct.vec, sk.vec));
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(
        const rbp::DlogTable<C>& table, const PreparedSk<C>& sk, const Ct<C>& ct
    ){
        return table.find(rbp::pair(ct.vec, sk.vec));
    }
}

#endif
