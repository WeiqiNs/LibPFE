#include "group.h"

void gen(ep_st *x) {
    g1_get_gen(x);
}

void g_mul(ep_st *r, ep_st *x, struct zp y) {
    g1_mul(r, x, y.point);
}

void gt_raise(fp_t *r, fp_t *x, struct zp y) {
    gt_exp(r, x, y.point);
}

void bp_map(ep_st *a, ep_st *b, fp_t *r) {
    pc_map(r, a, b);
}