#ifndef PFE_TEST_SCHEMES_HPP
#define PFE_TEST_SCHEMES_HPP

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <PFE/ipfe_bjk.hpp>
#include <PFE/ipfe_kim.hpp>
#include <PFE/ipfe_kks.hpp>
#include <PFE/ipfe_lin.hpp>
#include <PFE/ipfe_opt.hpp>
#include <PFE/ipfe_tao.hpp>
#include <PFE/qfe_bcfg.hpp>
#include <PFE/qfe_sgp.hpp>

template <class C>
auto table_decryptor(const rbp::Gt<C>& base, const std::int64_t lo, const std::int64_t hi){
    return [table = rbp::DlogTable<C>(base, lo, hi)](const auto& sk, const auto& ct){ return dec(table, sk, ct); };
}

inline auto range_decryptor(const std::int64_t lo, const std::int64_t hi){
    return [lo, hi](const auto& sk, const auto& ct){ return dec(sk, ct, lo, hi); };
}

template <class C>
struct Bjk{
    static constexpr std::string_view name = "Bishop et al.";
    static auto setup(const std::size_t n){ return IPFE::BJK::setup<C>(n); }
    static auto decryptor(const IPFE::BJK::Msk<C>&, const std::int64_t lo, const std::int64_t hi){
        return range_decryptor(lo, hi);
    }
};

template <class C>
struct Tao{
    static constexpr std::string_view name = "Tomida et al.";
    static auto setup(const std::size_t n){ return IPFE::TAO::setup<C>(n); }
    static auto decryptor(const IPFE::TAO::Msk<C>& msk, const std::int64_t lo, const std::int64_t hi){
        return table_decryptor<C>(msk.base, lo, hi);
    }
};

template <class C>
struct Kim{
    static constexpr std::string_view name = "Kim et al.";
    static auto setup(const std::size_t n){ return IPFE::KIM::setup<C>(n); }
    static auto decryptor(const IPFE::KIM::Msk<C>&, const std::int64_t lo, const std::int64_t hi){
        return range_decryptor(lo, hi);
    }
};

template <class C>
struct Lin{
    static constexpr std::string_view name = "Lin";
    static auto setup(const std::size_t n){ return IPFE::LIN::setup<C>(n); }
    static auto decryptor(const IPFE::LIN::Msk<C>&, const std::int64_t lo, const std::int64_t hi){
        return table_decryptor<C>(IPFE::LIN::base<C>(), lo, hi);
    }
};

template <class C>
struct Kks{
    static constexpr std::string_view name = "Kim, Kim and Seo";
    static auto setup(const std::size_t n){ return IPFE::KKS::setup<C>(n); }
    static auto decryptor(const IPFE::KKS::Msk<C>&, const std::int64_t lo, const std::int64_t hi){
        return table_decryptor<C>(IPFE::KKS::base<C>(), lo, hi);
    }
};

template <class C>
struct Opt{
    static constexpr std::string_view name = "Ojaswi et al.";
    static auto setup(const std::size_t n){ return IPFE::OPT::setup<C>(n); }
    static auto decryptor(const IPFE::OPT::Msk<C>&, const std::int64_t lo, const std::int64_t hi){
        return table_decryptor<C>(IPFE::OPT::base<C>(), lo, hi);
    }
};

template <class C>
struct Bcfg{
    static constexpr std::string_view name = "Baltico et al.";
    static auto setup(const std::size_t n){ return QFE::BCFG::setup<C>(n); }
    static auto decryptor(const QFE::BCFG::Keys<C>& keys, const std::int64_t lo, const std::int64_t hi){
        return [pk = keys.pk, table = rbp::DlogTable<C>(QFE::BCFG::base<C>(), lo, hi)](const auto& sk, const auto& ct){
            return QFE::BCFG::dec(table, pk, sk, ct);
        };
    }
};

template <class C>
struct Sgp{
    static constexpr std::string_view name = "Dufour-Sans et al.";
    static auto setup(const std::size_t n){ return QFE::SGP::setup<C>(n); }
    static auto decryptor(const QFE::SGP::Keys<C>&, const std::int64_t lo, const std::int64_t hi){
        return table_decryptor<C>(QFE::SGP::base<C>(), lo, hi);
    }
};

#endif
