#ifndef PFE_IPFE_BJK_HPP
#define PFE_IPFE_BJK_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include "ipfe.hpp"

namespace IPFE::BJK{
    template <class C>
    struct Msk{
        rbp::Matrix<C> b;
        rbp::Matrix<C> bi;
        rbp::Matrix<C> d;
        rbp::Matrix<C> di;
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
    struct PreparedSk{
        rbp::PreparedG2<C> r;
        rbp::PreparedG2<C> vec;
    };

    template <class C>
    [[nodiscard]] Msk<C> setup(const std::size_t size){
        auto b = rbp::Matrix<C>::random(2 * size + 4, 2 * size + 4);
        auto bi = b.inverse().transpose();
        auto d = rbp::Matrix<C>::random(2, 2);
        auto di = d.inverse().transpose();
        return {std::move(b), std::move(bi), std::move(d), std::move(di)};
    }

    template <class C>
    [[nodiscard]] Sk<C> keygen(const Msk<C>& msk, const IntVec& function){
        const auto f = to_vector<C>(function);
        const auto beta = rbp::Zp<C>::random();
        const auto beta_t = rbp::Zp<C>::random();
        const auto encoded = rbp::concat(rbp::concat(f * beta, f * beta_t), rbp::Vector<C>{{}, beta, {}, beta_t});
        return {
            rbp::G2<C>::mul_generator(rbp::Vector<C>{beta, beta_t} * msk.d),
            rbp::G2<C>::mul_generator(encoded * msk.b)
        };
    }

    template <class C>
    [[nodiscard]] Ct<C> enc(const Msk<C>& msk, const IntVec& message){
        const auto m = to_vector<C>(message);
        const auto alpha = rbp::Zp<C>::random();
        const auto alpha_t = rbp::Zp<C>::random();
        const auto encoded = rbp::concat(rbp::concat(m * alpha, m * alpha_t), rbp::Vector<C>{alpha, {}, alpha_t, {}});
        return {
            rbp::G1<C>::mul_generator(rbp::Vector<C>{alpha, alpha_t} * msk.di),
            rbp::G1<C>::mul_generator(encoded * msk.bi)
        };
    }

    template <class C>
    [[nodiscard]] PreparedSk<C> prepare(const Sk<C>& sk){
        return {rbp::PreparedG2<C>(sk.r), rbp::PreparedG2<C>(sk.vec)};
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(
        const Sk<C>& sk, const Ct<C>& ct, const std::int64_t lower_bound, const std::int64_t upper_bound
    ){
        return rbp::dlog(rbp::pair(ct.r, sk.r), rbp::pair(ct.vec, sk.vec), lower_bound, upper_bound);
    }

    template <class C>
    [[nodiscard]] std::optional<std::int64_t> dec(
        const PreparedSk<C>& sk, const Ct<C>& ct, const std::int64_t lower_bound, const std::int64_t upper_bound
    ){
        return rbp::dlog(rbp::pair(ct.r, sk.r), rbp::pair(ct.vec, sk.vec), lower_bound, upper_bound);
    }
}

#endif
