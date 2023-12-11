#include "sym_vector.hpp"

sym::gVec sym::get_g_pre_table(g base) {
    auto table = (sym::gVec) malloc(sizeof(sym::g) * RLC_G1_TABLE);
    g1_mul_pre(table, base);
    return table;
}

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

sym::gVec sym::vector_raise_with_table(sym::gVec table, sym::zpVec x, int size) {
    auto r = (sym::gVec) malloc(sizeof(sym::g) * size);
    for (int i = 0; i < size; i++) g1_mul_fix(r[i], table, x[i].num);
    return r;
}

void sym::inner_product(sym::gt r, sym::gVec a, sym::gVec b, int size) {
    pc_map_sim(r, a, b, size);
}