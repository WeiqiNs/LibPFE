#ifndef PFE_TEST_SCHEME_TYPES_HPP
#define PFE_TEST_SCHEME_TYPES_HPP

#include <tuple>
#include <utility>
#include <curves.hpp>
#include "schemes.hpp"

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
