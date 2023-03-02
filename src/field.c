#include "field.h"

struct zp new_zp() {
    struct zp result;
    bn_new(result.point);
    bn_new(result.modular);
    return result;
}

struct zp rand_zp(bn_st *modular) {
    struct zp result = new_zp();
    bn_rand_mod(result.point, modular);
    bn_copy(result.modular, modular);
    return result;
}

struct zp zp_zero(bn_st *modular) {
    struct zp result = new_zp();
    bn_set_dig(result.point, 0);
    bn_copy(result.modular, modular);
    return result;
}

struct zp zp_one(bn_st *modular) {
    struct zp result = new_zp();
    bn_set_dig(result.point, 1);
    bn_copy(result.modular, modular);
    return result;
}

struct zp zp_copy(struct zp x) {
    struct zp result = new_zp();
    bn_copy(result.point, x.point);
    bn_copy(result.modular, x.modular);
    return result;
}

struct zp zp_from_int(int x, bn_st *modular) {
    struct zp result = new_zp();
    bn_set_dig(result.point, x);
    bn_copy(result.modular, modular);
    return result;
}

struct zp zp_add(struct zp x, struct zp y) {
    struct zp result = new_zp();
    bn_add(result.point, x.point, y.point);
    bn_mod(result.point, result.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

struct zp zp_neg(struct zp x) {
    struct zp result = new_zp();
    bn_neg(result.point, x.point);
    bn_mod(result.point, result.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

struct zp zp_mul(struct zp x, struct zp y) {
    struct zp result = new_zp();
    bn_mul(result.point, x.point, y.point);
    bn_mod(result.point, result.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

struct zp zp_inv(struct zp x) {
    struct zp result = new_zp();
    bn_mod_inv(result.point, x.point, x.modular);
    bn_copy(result.modular, x.modular);
    return result;
}

int zp_cmp(struct zp x, struct zp y) {
    return bn_cmp(x.point, y.point) == RLC_EQ;
}

int zp_cmp_int(struct zp x, int y) {
    return bn_cmp_dig(x.point, y) == RLC_EQ;
}