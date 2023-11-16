#include "asym_vector.hpp"

asym::zpVec asym::vector_zp_from_int(const int *int_vec, int size, point mod) {
    zpVec x;
    x = (zpVec) malloc(sizeof(Zp) * size);
    for (int i = 0; i < size; i++) x[i] = zp_from_int(int_vec[i], mod);
    return x;
}

asym::zpVec asym::vector_zp_rand(int size, point mod) {
    zpVec x;
    x = (zpVec) malloc(sizeof(Zp) * size);
    for (int i = 0; i < size; i++) x[i] = zp_rand(mod);
    return x;
}

asym::zpVec asym::vector_merge(zpVec a, zpVec b, int size_a, int size_b) {
    zpVec r;
    r = (zpVec) malloc(sizeof(Zp) * (size_a + size_b));
    for (int i = 0; i < size_a; i++) r[i] = zp_copy(a[i]);
    for (int i = 0; i < size_b; i++) r[i + size_a] = zp_copy(b[i]);
    return r;
}

asym::zpVec asym::vector_add(zpVec a, zpVec b, int size) {
    zpVec r;
    r = (zpVec) malloc(sizeof(Zp) * size);
    for (int i = 0; i < size; i++) r[i] = zp_add(a[i], b[i]);
    return r;
}

asym::g1Vec asym::vector_raise_g1(g1 base, zpVec x, int size) {
    g1Vec r;
    r = (g1Vec) malloc(sizeof(g1) * size);
    for (int i = 0; i < size; i++) g1_mul(r[i], base, x[i]);
    return r;
}

asym::g2Vec asym::vector_raise_g2(g2 base, zpVec x, int size) {
    g2Vec r;
    r = (g2Vec) malloc(sizeof(g2) * size);
    for (int i = 0; i < size; i++) g2_mul(r[i], base, x[i]);
    return r;
}

void asym::inner_product(gt r, g1Vec a, g2Vec b, int size) {
    gt temp;
    gt_set_unity(r);
    for (int i = 0; i < size; i++) {
        bp_map(temp, a[i], b[i]);
        gt_mul(r, r, temp);
    }
}