#ifndef PPANN_FIELD_H
#define PPANN_FIELD_H

#include <gmp.h>

extern "C" {
#include "relic/relic.h"
}

struct zp {
    bn_t point{};
    bn_t modular{};
};

zp rand_zp(bn_t modular);

zp zp_zero(bn_t modular);

zp zp_one(bn_t modular);

zp zp_copy(zp x);

zp zp_from_int(int x, bn_t modular);

zp zp_add(zp x, zp y);

zp zp_neg(zp x);

zp zp_mul(zp x, zp y);

zp zp_inv(zp x);

int zp_cmp(zp x, zp y);

int zp_cmp_int(zp x, int y);

#endif //PPANN_FIELD_H