#ifndef PFE_IPFE_LIN_HPP
#define PFE_IPFE_LIN_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include "ipfe.hpp"

namespace IPFE::LIN{
    template <class C>
    struct Msk{
        rbp::Vector<C> s1;
        rbp::Vector<C> s2;
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
        return {rbp::random_vector<C>(2 * size), rbp::random_vector<C>(2 * size + 1)};
    }

    template <class C>
    [[nodiscard]] rbp::Gt<C> base(){
        return rbp::Gt<C>::generator();
    }

    template <class C>
    [[nodiscard]] Sk<C> keygen(const Msk<C>& msk, const IntVec& function){
        const auto f = rbp::concat(to_vector<C>(function), rbp::Vector<C>(function.size()));
        const auto key = rbp::concat(rbp::Vector<C>{rbp::inner(f, msk.s1)}, f);
        const auto r = rbp::Zp<C>::random();
        return {rbp::G2<C>::mul_generator(rbp::concat(rbp::Vector<C>{-r}, msk.s2 * r + key))};
    }

    template <class C>
    [[nodiscard]] Ct<C> enc(const Msk<C>& msk, const IntVec& message){
        const auto m = rbp::concat(to_vector<C>(message), rbp::Vector<C>(message.size()));
        const auto r = rbp::Zp<C>::random();
        const auto ct = rbp::concat(rbp::Vector<C>{-r}, msk.s1 * r + m);
        return {rbp::G1<C>::mul_generator(rbp::concat(rbp::Vector<C>{rbp::inner(msk.s2, ct)}, ct))};
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(const rbp::DlogTable<C>& table, const Sk<C>& sk, const Ct<C>& ct){
        return table.find(rbp::pair(ct.vec, sk.vec));
    }
}

#endif
