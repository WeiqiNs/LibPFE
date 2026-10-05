#ifndef PFE_IPFE_KIM_HPP
#define PFE_IPFE_KIM_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include "ipfe.hpp"

namespace IPFE::KIM{
    template <class C>
    struct Msk{
        rbp::Zp<C> det;
        rbp::Matrix<C> b;
        rbp::Matrix<C> bi;
    };

    template <class C>
    struct Sk{
        rbp::G2<C> r;
        std::vector<rbp::G2<C>> vec;
    };

    template <class C>
    struct Ct{
        rbp::G1<C> r;
        std::vector<rbp::G1<C>> vec;
    };

    template <class C>
    [[nodiscard]] Msk<C> setup(const std::size_t size){
        auto b = rbp::Matrix<C>::random(size, size);
        auto [inverse, det] = b.inverse_with_determinant();
        return {det, std::move(b), (inverse * det).transpose()};
    }

    template <class C>
    [[nodiscard]] Sk<C> keygen(const Msk<C>& msk, const IntVec& function){
        const auto alpha = rbp::Zp<C>::random();
        return {
            rbp::G2<C>::mul_generator(alpha * msk.det),
            rbp::G2<C>::mul_generator(to_vector<C>(function) * alpha * msk.b)
        };
    }

    template <class C>
    [[nodiscard]] Ct<C> enc(const Msk<C>& msk, const IntVec& message){
        const auto beta = rbp::Zp<C>::random();
        return {rbp::G1<C>::mul_generator(beta), rbp::G1<C>::mul_generator(to_vector<C>(message) * beta * msk.bi)};
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(
        const Sk<C>& sk, const Ct<C>& ct, const std::int64_t lower_bound, const std::int64_t upper_bound
    ){
        return rbp::dlog(rbp::pair(ct.r, sk.r), rbp::pair(ct.vec, sk.vec), lower_bound, upper_bound);
    }
}

#endif
