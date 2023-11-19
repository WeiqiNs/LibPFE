#include "sym_field.hpp"

sym::Zp sym::zp_rand(sym::point mod) {
    sym::Zp result{};
    bn_rand_mod(result.num, mod);
    bn_copy(result.mod, mod);
    return result;
}

sym::Zp sym::zp_zero(sym::point mod) {
    sym::Zp result{};
    bn_set_dig(result.num, 0);
    bn_copy(result.mod, mod);
    return result;
}

sym::Zp sym::zp_one(sym::point mod) {
    sym::Zp result{};
    bn_set_dig(result.num, 1);
    bn_copy(result.mod, mod);
    return result;
}

sym::Zp sym::zp_copy(sym::Zp x) {
    sym::Zp result{};
    bn_copy(result.num, x.num);
    bn_copy(result.mod, x.mod);
    return result;
}

sym::Zp sym::zp_from_int(int x, sym::point mod) {
    sym::Zp result{};
    bn_set_dig(result.num, x);
    bn_copy(result.mod, mod);
    return result;
}

sym::Zp sym::zp_add(sym::Zp x, sym::Zp y) {
    sym::Zp result{};
    bn_add(result.num, x.num, y.num);
    bn_mod(result.num, result.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

sym::Zp sym::zp_neg(sym::Zp x) {
    sym::Zp result{};
    bn_neg(result.num, x.num);
    bn_mod(result.num, result.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

sym::Zp sym::zp_mul(sym::Zp x, sym::Zp y) {
    sym::Zp result{};
    bn_mul(result.num, x.num, y.num);
    bn_mod(result.num, result.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

sym::Zp sym::zp_inv(sym::Zp x) {
    sym::Zp result{};
    bn_mod_inv(result.num, x.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

int sym::zp_cmp(sym::Zp x, sym::Zp y) {
    return bn_cmp(x.num, y.num) == RLC_EQ;
}

int sym::zp_cmp_int(sym::Zp x, int y) {
    return bn_cmp_dig(x.num, y) == RLC_EQ;
}