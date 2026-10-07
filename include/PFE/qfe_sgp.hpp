#ifndef PFE_QFE_SGP_HPP
#define PFE_QFE_SGP_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include "qfe.hpp"

namespace QFE::SGP{
    template <class C>
    struct Msk{
        rbp::Vector<C> s;
        rbp::Vector<C> t;
    };

    template <class C>
    struct Pk{
        std::vector<rbp::G1<C>> s;
        std::vector<rbp::G2<C>> t;
    };

    template <class C>
    struct Keys{
        Pk<C> pk;
        Msk<C> msk;
    };

    template <class C>
    struct Sk{
        rbp::Matrix<C> f;
        rbp::G2<C> key;
    };

    template <class C>
    struct PreparedSk{
        rbp::Matrix<C> f;
        rbp::PreparedG2<C> key;
    };

    template <class C>
    struct Ct{
        rbp::G1<C> gamma;
        std::vector<rbp::G1<C>> a0;
        std::vector<rbp::G1<C>> a1;
        std::vector<rbp::G2<C>> b0;
        std::vector<rbp::G2<C>> b1;
    };

    namespace detail{
        template <class C, class Key>
        [[nodiscard]] std::optional<std::int64_t> decrypt(
            const rbp::DlogTable<C>& table, const rbp::Matrix<C>& f, const Key& key, const Ct<C>& ct
        ){
            rbp::PairingProduct<C> product;
            product.add(std::vector{ct.gamma}, key);
            QFE::detail::add_bilinear(product, ct.a0, f, ct.b0);
            QFE::detail::add_bilinear(product, ct.a1, f, ct.b1);
            return table.find(product.evaluate());
        }
    }

    template <class C>
    [[nodiscard]] Keys<C> setup(const std::size_t size){
        auto s = rbp::random_vector<C>(size);
        auto t = rbp::random_vector<C>(size);
        return {{rbp::G1<C>::mul_generator(s), rbp::G2<C>::mul_generator(t)}, {std::move(s), std::move(t)}};
    }

    template <class C>
    [[nodiscard]] rbp::Gt<C> base(){
        return rbp::Gt<C>::generator();
    }

    template <class C>
    [[nodiscard]] Sk<C> keygen(const Msk<C>& msk, const IntMat& function){
        auto f = to_matrix<C>(function);
        auto key = rbp::G2<C>::mul_generator(rbp::inner(msk.s, f * msk.t));
        return {std::move(f), std::move(key)};
    }

    template <class C>
    [[nodiscard]] Ct<C> enc(const Pk<C>& pk, const IntVec& left, const IntVec& right){
        const auto x = to_vector<C>(left);
        const auto y = to_vector<C>(right);
        const auto gamma = rbp::Zp<C>::random();
        const auto w = rbp::Matrix<C>::random(2, 2);
        const auto wi = w.inverse().transpose();
        return {
            rbp::G1<C>::mul_generator(gamma),
            QFE::detail::masked(pk.s, gamma * wi.at(0, 1), x * wi.at(0, 0)),
            QFE::detail::masked(pk.s, gamma * wi.at(1, 1), x * wi.at(1, 0)),
            QFE::detail::masked(pk.t, -w.at(0, 1), y * w.at(0, 0)),
            QFE::detail::masked(pk.t, -w.at(1, 1), y * w.at(1, 0))
        };
    }

    template <class C>
    [[nodiscard]] PreparedSk<C> prepare(const Sk<C>& sk){
        return {sk.f, rbp::PreparedG2<C>(std::vector{sk.key})};
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(const rbp::DlogTable<C>& table, const Sk<C>& sk, const Ct<C>& ct){
        return detail::decrypt(table, sk.f, std::vector{sk.key}, ct);
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(
        const rbp::DlogTable<C>& table, const PreparedSk<C>& sk, const Ct<C>& ct
    ){
        return detail::decrypt(table, sk.f, sk.key, ct);
    }
}

#endif
