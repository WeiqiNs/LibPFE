#ifndef RIPFE_TEST_SCHEMES_HPP
#define RIPFE_TEST_SCHEMES_HPP

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <utility>
#include <curves.hpp>
#include <RIPFE/ipfe_bjk.hpp>
#include <RIPFE/ipfe_kim.hpp>
#include <RIPFE/ipfe_kks.hpp>
#include <RIPFE/ipfe_lin.hpp>
#include <RIPFE/ipfe_opt.hpp>
#include <RIPFE/ipfe_tao.hpp>
#include <RIPFE/qfe_bcfg.hpp>
#include <RIPFE/qfe_sgp.hpp>

template <class C>
auto table_decryptor(const rbp::Gt<C>& base, const std::int64_t lo, const std::int64_t hi){
    return [table = rbp::DlogTable<C>(base, lo, hi)](const auto& sk, const auto& ct){ return dec(table, sk, ct); };
}

inline auto range_decryptor(const std::int64_t lo, const std::int64_t hi){
    return [lo, hi](const auto& sk, const auto& ct){ return dec(sk, ct, lo, hi); };
}

template <class C>
struct Bjk{
    static auto setup(const std::size_t n){ return IPFE::BJK::setup<C>(n); }
    static auto decryptor(const IPFE::BJK::Msk<C>&, const std::int64_t lo, const std::int64_t hi){
        return range_decryptor(lo, hi);
    }
};

template <class C>
struct Tao{
    static auto setup(const std::size_t n){ return IPFE::TAO::setup<C>(n); }
    static auto decryptor(const IPFE::TAO::Msk<C>& msk, const std::int64_t lo, const std::int64_t hi){
        return table_decryptor<C>(msk.base, lo, hi);
    }
};

template <class C>
struct Kim{
    static auto setup(const std::size_t n){ return IPFE::KIM::setup<C>(n); }
    static auto decryptor(const IPFE::KIM::Msk<C>&, const std::int64_t lo, const std::int64_t hi){
        return range_decryptor(lo, hi);
    }
};

template <class C>
struct Lin{
    static auto setup(const std::size_t n){ return IPFE::LIN::setup<C>(n); }
    static auto decryptor(const IPFE::LIN::Msk<C>&, const std::int64_t lo, const std::int64_t hi){
        return table_decryptor<C>(IPFE::LIN::base<C>(), lo, hi);
    }
};

template <class C>
struct Kks{
    static auto setup(const std::size_t n){ return IPFE::KKS::setup<C>(n); }
    static auto decryptor(const IPFE::KKS::Msk<C>&, const std::int64_t lo, const std::int64_t hi){
        return table_decryptor<C>(IPFE::KKS::base<C>(), lo, hi);
    }
};

template <class C>
struct Opt{
    static auto setup(const std::size_t n){ return IPFE::OPT::setup<C>(n); }
    static auto decryptor(const IPFE::OPT::Msk<C>&, const std::int64_t lo, const std::int64_t hi){
        return table_decryptor<C>(IPFE::OPT::base<C>(), lo, hi);
    }
};

template <class C>
struct Bcfg{
    static auto setup(const std::size_t n){ return QFE::BCFG::setup<C>(n); }
    static auto decryptor(const QFE::BCFG::Keys<C>& keys, const std::int64_t lo, const std::int64_t hi){
        return [pk = keys.pk, table = rbp::DlogTable<C>(QFE::BCFG::base<C>(), lo, hi)](const auto& sk, const auto& ct){
            return QFE::BCFG::dec(table, pk, sk, ct);
        };
    }
};

template <class C>
struct Sgp{
    static auto setup(const std::size_t n){ return QFE::SGP::setup<C>(n); }
    static auto decryptor(const QFE::SGP::Keys<C>&, const std::int64_t lo, const std::int64_t hi){
        return table_decryptor<C>(QFE::SGP::base<C>(), lo, hi);
    }
};

template <class Tuple>
struct AsTypes;

template <class... Ts>
struct AsTypes<std::tuple<Ts...>>{
    using type = ::testing::Types<Ts...>;
};

template <template <class> class Scheme, class... Cs>
using Instances = std::tuple<Scheme<Cs>...>;

template <class Curves, template <class> class... Schemes>
struct OnEveryCurve;

template <class... Cs, template <class> class... Schemes>
struct OnEveryCurve<std::tuple<Cs...>, Schemes...>{
    using type = typename AsTypes<decltype(std::tuple_cat(std::declval<Instances<Schemes, Cs...>>()...))>::type;
};

using InnerProductSchemes = OnEveryCurve<CurveTuple, Bjk, Tao, Kim, Lin, Kks, Opt>::type;
using QuadraticSchemes = OnEveryCurve<CurveTuple, Bcfg, Sgp>::type;

#endif
