#include "sym_vector.hpp"

sym::zpVec sym::vector_zp_from_int(const int *int_vec, int size, sym::point mod) {
    auto x = (sym::zpVec) malloc(sizeof(sym::Zp) * size);
    for (int i = 0; i < size; i++) x[i] = sym::zp_from_int(int_vec[i], mod);
    return x;
}

sym::zpVec sym::vector_zp_rand(int size, sym::point mod) {
    auto x = (sym::zpVec) malloc(sizeof(sym::Zp) * size);
    for (int i = 0; i < size; i++) x[i] = sym::zp_rand(mod);
    return x;
}

sym::zpVec sym::vector_join(sym::zpVec a, sym::zpVec b, int size_a, int size_b) {
    auto r = (sym::zpVec) malloc(sizeof(sym::Zp) * (size_a + size_b));
    for (int i = 0; i < size_a; i++) r[i] = sym::zp_copy(a[i]);
    for (int i = 0; i < size_b; i++) r[i + size_a] = sym::zp_copy(b[i]);
    return r;
}

sym::zpVec sym::vector_add(sym::zpVec a, sym::zpVec b, int size) {
    auto r = (sym::zpVec) malloc(sizeof(sym::Zp) * size);
    for (int i = 0; i < size; i++) r[i] = sym::zp_add(a[i], b[i]);
    return r;
}

sym::gVec sym::vector_raise(sym::g base, sym::zpVec x, int size) {
    auto r = (sym::gVec) malloc(sizeof(sym::g) * size);
    for (int i = 0; i < size; i++) sym::g_mul(r[i], base, x[i]);
    return r;
}

void sym::inner_product(sym::gt r, sym::gVec a, sym::gVec b, int size) {
    sym::gt temp;
    sym::gt_get_unity(r);
    for (int i = 0; i < size; i++) {
        sym::bp_map(temp, a[i], b[i]);
        sym::gt_multiply(r, r, temp);
    }
}