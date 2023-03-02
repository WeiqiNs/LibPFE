#ifndef PPANN_FIELD_H
#define PPANN_FIELD_H

#include "relic/relic.h"

struct zp {
    bn_t point;
    bn_t modular;
};

struct zp new_zp();

struct zp rand_zp(bn_t modular);

struct zp zp_zero(bn_t modular);

struct zp zp_one(bn_t modular);

struct zp zp_copy(struct zp x);

struct zp zp_from_int(int x, bn_t modular);

struct zp zp_add(struct zp x, struct zp y);

struct zp zp_neg(struct zp x);

struct zp zp_mul(struct zp x, struct zp y);

struct zp zp_inv(struct zp x);

int zp_cmp(struct zp x, struct zp y);

int zp_cmp_int(struct zp x, int y);

#endif //PPANN_FIELD_H