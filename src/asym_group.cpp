#include "asym_group.hpp"

void asym::g1_gen(g1 x) {
    g1_get_gen(x);
}

void asym::g2_gen(g2 x) {
    g2_get_gen(x);
}

void asym::g1_mul(g1 r, g1 x, Zp y) {
    g1_mul(r, x, y.num);
}

void asym::g2_mul(g2 r, g2 x, Zp y) {
    g2_mul(r, x, y.num);
}

void asym::gt_raise(gt r, gt x, Zp y) {
    gt_exp(r, x, y.num);
}

void asym::bp_map(gt r, g1 x, g2 y) {
    pc_map(r, x, y);
}

