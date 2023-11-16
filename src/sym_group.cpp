#include "sym_group.hpp"

void sym::g_gen(g x) {
    g1_get_gen(x);
}

void sym::g_mul(g r, g x, Zp y) {
    g1_mul(r, x, y.num);
}

void sym::gt_raise(gt r, gt x, Zp y) {
    gt_exp(r, x, y.num);
}

void sym::bp_map(gt r, g a, g b) {
    pc_map(r, a, b);
}