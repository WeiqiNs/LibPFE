#ifndef PFE_IPFE_TAO_HPP
#define PFE_IPFE_TAO_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include "ipfe.hpp"

namespace IPFE::TAO{
    template <class C>
    struct Msk{
        rbp::Matrix<C> b;
        rbp::Matrix<C> bi;
        rbp::Gt<C> base;
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

    template <class C>
    [[nodiscard]] Msk<C> setup(const std::size_t size){
        const auto r = rbp::Zp<C>::random();
        auto b = rbp::Matrix<C>::random(2 * size + 5, 2 * size + 5);
        auto bi = (b.inverse() * r).transpose();
        return {std::move(b), std::move(bi), rbp::Gt<C>::generator().pow(r)};
    }

    template <class C>
    [[nodiscard]] Sk<C> keygen(const Msk<C>& msk, const IntVec& function){
        const auto f = to_vector<C>(function);
        const auto tail = rbp::Vector<C>{rbp::Zp<C>::random(), rbp::Zp<C>::random(), {}};
        const auto encoded = rbp::concat(rbp::concat(f, rbp::Vector<C>(f.size() + 2)), tail);
        return {rbp::G2<C>::mul_generator(encoded * msk.b)};
    }

    template <class C>
    [[nodiscard]] Ct<C> enc(const Msk<C>& msk, const IntVec& message){
        const auto m = to_vector<C>(message);
        const auto tail = rbp::Vector<C>{rbp::Zp<C>::random(), rbp::Zp<C>::random(), {}, {}, {}};
        const auto encoded = rbp::concat(rbp::concat(m, rbp::Vector<C>(m.size())), tail);
        return {rbp::G1<C>::mul_generator(encoded * msk.bi)};
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
