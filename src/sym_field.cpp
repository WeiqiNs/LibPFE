#include "sym_field.hpp"

sym::Zp sym::zp_rand(point mod) {
    Zp result;
    bn_rand_mod(result.num, mod);
    bn_copy(result.mod, mod);
    return result;
}

sym::Zp sym::zp_zero(point mod) {
    Zp result;
    bn_set_dig(result.num, 0);
    bn_copy(result.mod, mod);
    return result;
}

sym::Zp sym::zp_one(point mod) {
    Zp result;
    bn_set_dig(result.num, 1);
    bn_copy(result.mod, mod);
    return result;
}

sym::Zp sym::zp_copy(Zp x) {
    Zp result;
    bn_copy(result.num, x.num);
    bn_copy(result.mod, x.mod);
    return result;
}

sym::Zp sym::zp_from_int(int x, point mod) {
    Zp result;
    bn_set_dig(result.num, x);
    bn_copy(result.mod, mod);
    return result;
}

sym::Zp sym::zp_add(Zp x, Zp y) {
    Zp result;
    bn_add(result.num, x.num, y.num);
    bn_mod(result.num, result.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

sym::Zp sym::zp_neg(Zp x) {
    Zp result;
    bn_neg(result.num, x.num);
    bn_mod(result.num, result.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

sym::Zp sym::zp_mul(Zp x, Zp y) {
    Zp result;
    bn_mul(result.num, x.num, y.num);
    bn_mod(result.num, result.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

sym::Zp sym::zp_inv(Zp x) {
    Zp result;
    bn_mod_inv(result.num, x.num, x.mod);
    bn_copy(result.mod, x.mod);
    return result;
}

int sym::zp_cmp(Zp x, Zp y) {
    return bn_cmp(x.num, y.num) == RLC_EQ;
}

int sym::zp_cmp_int(Zp x, int y) {
    return bn_cmp_dig(x.num, y) == RLC_EQ;
}