#include "field.h"

zp rand_zp(bn_st *modular) {
    zp result;
    bn_rand_mod(result.point, modular);
    bn_copy(result.modular, modular);
    return result;
}

zp zp_zero(bn_st *modular) {
    zp result;
    bn_set_dig(result.point, 0);
    bn_copy(result.modular, modular);
    return result;
}

zp zp_one(bn_st *modular) {
    zp result;
    bn_set_dig(result.point, 1);
    bn_copy(result.modular, modular);
    return result;
}

zp zp_copy(zp x) {
    zp result;
    bn_copy(result.point, x.point);
    bn_copy(result.modular, x.modular);
    return result;
}

zp zp_from_int(int x, bn_st *modular) {
    zp result;
    bn_set_dig(result.point, x);
    bn_copy(result.modular, modular);
    return result;
}

zp zp_add(zp x, zp y) {
    zp result;
    bn_add(result.point, x.point, y.point);
    bn_mod(result.point, result.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

zp zp_neg(zp x) {
    zp result;
    bn_neg(result.point, x.point);
    bn_mod(result.point, result.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

zp zp_mul(zp x, zp y) {
    zp result;
    bn_mul(result.point, x.point, y.point);
    bn_mod(result.point, result.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

zp zp_inv(zp x) {
    zp result;
    bn_mod_inv(result.point, x.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

int zp_cmp(zp x, zp y) {
    return bn_cmp(x.point, y.point) == RLC_EQ;
}

int zp_cmp_int(zp x, int y) {
    return bn_cmp_dig(x.point, y) == RLC_EQ;
}