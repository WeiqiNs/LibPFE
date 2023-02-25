#ifndef PPANN_FIELD_H
#define PPANN_FIELD_H

#include "relic/relic.h"

typedef fp_t zp;

void rand_zp(zp x);

void zp_zero(zp x);

void zp_copy(zp x_copy, zp x);

void zp_from_int(zp x, int x_int);

void zp_add(zp r, zp x, zp y);

void zp_multiply(zp p, zp x, zp y);

void zp_inverse(zp xi, zp x);

#endif //PPANN_FIELD_H