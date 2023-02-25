#include "group.h"

void generator(ep_st *x) {
    g1_get_gen(x);
}

void negate(ep_st *nx, ep_st *x) {
    g1_neg(nx, x);
}

void multiply(ep_st *r, ep_st *x, dig_t *y) {
    bn_t new_y;
    fp_prime_back(new_y, y);
    g1_mul(r, x, new_y);
}

void exponentiation(fp_t *r, fp_t *x, dig_t *y) {
    bn_t new_y;
    fp_prime_back(new_y, y);
    gt_exp(r, x, new_y);
}

void map(ep_st *a, ep_st *b, fp_t *r) {
    pc_map(r, a, b);
}
