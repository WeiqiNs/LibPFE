#ifndef PPANN_FIELD_H
#define PPANN_FIELD_H

#include <gmp.h>

extern "C" {
#include "relic_sym/relic.h"
}

struct ZP {
    bn_t point{};
    bn_t modular{};
};

ZP rand_zp(bn_t modular);

ZP zp_zero(bn_t modular);

ZP zp_one(bn_t modular);

ZP zp_copy(ZP x);

ZP zp_from_int(int x, bn_t modular);

ZP zp_add(ZP x, ZP y);

ZP zp_neg(ZP x);

ZP zp_mul(ZP x, ZP y);

ZP zp_inv(ZP x);

int zp_cmp(ZP x, ZP y);

int zp_cmp_int(ZP x, int y);

#endif //PPANN_FIELD_H