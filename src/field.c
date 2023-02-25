#include "field.h"

void rand_zp(dig_t *x) {
    fp_rand(x);
}

void zp_zero(dig_t *x) {
    fp_zero(x);
}

void zp_copy(dig_t *x_copy, dig_t *x) {
    fp_copy(x_copy, x);
}

void zp_from_int(dig_t *x, int y) {
    fp_set_dig(x, y);
}

void zp_add(dig_t *r, dig_t *x, dig_t *y) {
    fp_add(r, x, y);
}

void zp_multiply(dig_t *p, dig_t *x, dig_t *y) {
    fp_mul(p, x, y);
}

void zp_inverse(dig_t *xi, dig_t *x) {
    fp_inv(xi, x);
}