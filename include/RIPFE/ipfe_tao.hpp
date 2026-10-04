#ifndef RIPFE_IPFE_TAO_HPP
#define RIPFE_IPFE_TAO_HPP

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
    [[nodiscard]] Msk<C> setup(const std::size_t size){
        const auto r = rbp::Zp<C>::random();
        auto b = rbp::Matrix<C>::random(2 * size + 5, 2 * size + 5);
        auto bi = (b.inverse() * r).transpose();
        return {std::move(b), std::move(bi), rbp::Gt<C>::generator().pow(r)};
    }

    template <class C>
    [[nodiscard]] Sk<C> keygen(const Msk<C>& msk, const IntVec& function){
        auto f = to_vector<C>(function);
        f.resize(2 * function.size() + 2);
        f.push_back(rbp::Zp<C>::random());
        f.push_back(rbp::Zp<C>::random());
        f.emplace_back();
        return {rbp::G2<C>::mul_generator(f * msk.b)};
    }

    template <class C>
    [[nodiscard]] Ct<C> enc(const Msk<C>& msk, const IntVec& message){
        auto m = to_vector<C>(message);
        m.resize(2 * message.size());
        m.push_back(rbp::Zp<C>::random());
        m.push_back(rbp::Zp<C>::random());
        m.resize(m.size() + 3);
        return {rbp::G1<C>::mul_generator(m * msk.bi)};
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(const rbp::DlogTable<C>& table, const Sk<C>& sk, const Ct<C>& ct){
        return table.find(rbp::pair(ct.vec, sk.vec));
    }
}

#endif
