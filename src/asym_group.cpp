#include "asym_group.hpp"

void asym::g1_gen(asym::g1 x) {
    g1_get_gen(x);
}

void asym::g2_gen(asym::g2 x) {
    g2_get_gen(x);
}

void asym::g1_mul(asym::g1 r, asym::g1 x, asym::Zp y) {
    g1_mul(r, x, y.num);
}

void asym::g2_mul(asym::g2 r, asym::g2 x, asym::Zp y) {
    g2_mul(r, x, y.num);
}

void asym::gt_get_unity(asym::gt x) {
    gt_set_unity(x);
}

void asym::gt_raise(asym::gt r, asym::gt x, asym::Zp y) {
    gt_exp(r, x, y.num);
}

void asym::gt_raise_int(asym::gt r, asym::gt x, int y) {
    gt_exp_dig(r, x, y);
}

void asym::gt_inverse(asym::gt r, asym::gt x) {
    gt_inv(r, x);
}

void asym::gt_multiply(asym::gt r, asym::gt x, asym::gt y) {
    gt_mul(r, x, y);
}

int asym::gt_compare(asym::gt x, asym::gt y) {
    return gt_cmp(x, y) == RLC_EQ;
}

void asym::bp_map(asym::gt r, asym::g1 x, asym::g2 y) {
    pc_map(r, x, y);
}