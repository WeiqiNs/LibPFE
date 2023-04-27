#include "field.h"

ZP rand_zp(bn_st *modular) {
    ZP result;
    bn_rand_mod(result.point, modular);
    bn_copy(result.modular, modular);
    return result;
}

ZP zp_zero(bn_st *modular) {
    ZP result;
    bn_set_dig(result.point, 0);
    bn_copy(result.modular, modular);
    return result;
}

ZP zp_one(bn_st *modular) {
    ZP result;
    bn_set_dig(result.point, 1);
    bn_copy(result.modular, modular);
    return result;
}

ZP zp_copy(ZP x) {
    ZP result;
    bn_copy(result.point, x.point);
    bn_copy(result.modular, x.modular);
    return result;
}

ZP zp_from_int(int x, bn_st *modular) {
    ZP result;
    bn_set_dig(result.point, x);
    bn_copy(result.modular, modular);
    return result;
}

ZP zp_add(ZP x, ZP y) {
    ZP result;
    bn_add(result.point, x.point, y.point);
    bn_mod(result.point, result.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

ZP zp_neg(ZP x) {
    ZP result;
    bn_neg(result.point, x.point);
    bn_mod(result.point, result.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

ZP zp_mul(ZP x, ZP y) {
    ZP result;
    bn_mul(result.point, x.point, y.point);
    bn_mod(result.point, result.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

ZP zp_inv(ZP x) {
    ZP result;
    bn_mod_inv(result.point, x.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

int zp_cmp(ZP x, ZP y) {
    return bn_cmp(x.point, y.point) == RLC_EQ;
}

int zp_cmp_int(ZP x, int y) {
    return bn_cmp_dig(x.point, y) == RLC_EQ;
}