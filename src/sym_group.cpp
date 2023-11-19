#include "sym_group.hpp"

void sym::g_gen(sym::g x) {
    g1_get_gen(x);
}

void sym::g_mul(sym::g r, sym::g x, sym::Zp y) {
    g1_mul(r, x, y.num);
}

void sym::gt_get_unity(sym::gt x) {
    gt_set_unity(x);
}

void sym::gt_raise(sym::gt r, sym::gt x, sym::Zp y) {
    gt_exp(r, x, y.num);
}

void sym::gt_raise_int(sym::gt r, sym::gt x, int y) {
    gt_exp_dig(r, x, y);
}

void sym::gt_inverse(sym::gt r, sym::gt x) {
    gt_inv(r, x);
}

void sym::gt_multiply(sym::gt r, sym::gt x, sym::gt y) {
    gt_mul(r, x, y);
}

int sym::gt_compare(sym::gt x, sym::gt y) {
    return gt_cmp(x, y) == RLC_EQ;
}

void sym::bp_map(sym::gt r, sym::g a, sym::g b) {
    pc_map(r, a, b);
}