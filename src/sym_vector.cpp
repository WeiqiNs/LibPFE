#include "sym_vector.hpp"

sym::zpVec sym::vector_zp_from_int(const int *int_vec, int size, point mod) {
    zpVec x;
    x = (zpVec) malloc(sizeof(Zp) * size);
    for (int i = 0; i < size; i++) x[i] = zp_from_int(int_vec[i], mod);
    return x;
}

sym::zpVec sym::vector_zp_rand(int size, point mod) {
    zpVec x;
    x = (zpVec) malloc(sizeof(Zp) * size);
    for (int i = 0; i < size; i++) x[i] = zp_rand(mod);
    return x;
}

sym::zpVec sym::vector_merge(zpVec a, zpVec b, int size_a, int size_b) {
    zpVec r;
    r = (zpVec) malloc(sizeof(Zp) * (size_a + size_b));
    for (int i = 0; i < size_a; i++) r[i] = zp_copy(a[i]);
    for (int i = 0; i < size_b; i++) r[i + size_a] = zp_copy(b[i]);
    return r;
}

sym::zpVec sym::vector_add(zpVec a, zpVec b, int size) {
    zpVec r;
    r = (zpVec) malloc(sizeof(Zp) * size);
    for (int i = 0; i < size; i++) r[i] = zp_add(a[i], b[i]);
    return r;
}

sym::gVec sym::vector_raise(g base, zpVec x, int size) {
    gVec r;
    r = (gVec) malloc(sizeof(g) * size);
    for (int i = 0; i < size; i++) g_mul(r[i], base, x[i]);
    return r;
}

void sym::inner_product(gt r, gVec a, gVec b, int size) {
    gt temp;
    gt_set_unity(r);
    for (int i = 0; i < size; i++) {
        bp_map(temp, a[i], b[i]);
        gt_mul(r, r, temp);
    }
}