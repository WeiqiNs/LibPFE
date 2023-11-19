#include "asym_field.hpp"

asym::Zp asym::zp_rand(asym::point mod) {
    asym::Zp result{};
    bn_rand_mod(result.num, mod);
    bn_copy(result.mod, mod);
    return result;
}

asym::Zp asym::zp_zero(asym::point mod) {
    asym::Zp result{};
    bn_set_dig(result.num, 0);
    bn_copy(result.mod, mod);
    return result;
}

asym::Zp asym::zp_one(asym::point mod) {
    asym::Zp result{};
    bn_set_dig(result.num, 1);
    bn_copy(result.mod, mod);
    return result;
}

asym::Zp asym::zp_copy(asym::Zp x) {
    asym::Zp result{};
    bn_copy(result.num, x.num);
    bn_copy(result.mod, x.mod);
    return result;
}

asym::Zp asym::zp_from_int(int x, asym::point mod) {
    asym::Zp result{};
    bn_set_dig(result.num, x);
    bn_copy(result.mod, mod);
    return result;
}

asym::Zp asym::zp_add(asym::Zp x, asym::Zp y) {
    asym::Zp result{};
    bn_add(result.num, x.num, y.num);
    bn_mod(result.num, result.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

asym::Zp asym::zp_neg(asym::Zp x) {
    asym::Zp result{};
    bn_neg(result.num, x.num);
    bn_mod(result.num, result.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

asym::Zp asym::zp_mul(asym::Zp x, asym::Zp y) {
    asym::Zp result{};
    bn_mul(result.num, x.num, y.num);
    bn_mod(result.num, result.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

asym::Zp asym::zp_inv(asym::Zp x) {
    asym::Zp result{};
    bn_mod_inv(result.num, x.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

int asym::zp_cmp(asym::Zp x, asym::Zp y) {
    return bn_cmp(x.num, y.num) == RLC_EQ;
}

int asym::zp_cmp_int(asym::Zp x, int y) {
    return bn_cmp_dig(x.num, y) == RLC_EQ;
}