#include "asym_vector.hpp"

asym::g1Vec asym::get_g1_pre_table(g1 base) {
    auto table = (asym::g1Vec) malloc(sizeof(asym::g1) * RLC_G1_TABLE);
    g1_mul_pre(table, base);
    return table;
}

asym::g2Vec asym::get_g2_pre_table(g2 base) {
    auto table = (asym::g2Vec) malloc(sizeof(asym::g2) * RLC_G1_TABLE);
    g2_mul_pre(table, base);
    return table;
}

asym::zpVec asym::vector_zp_from_int(const int *int_vec, int size, asym::point mod) {
    auto x = (asym::zpVec) malloc(sizeof(asym::Zp) * size);
    for (int i = 0; i < size; i++) x[i] = asym::zp_from_int(int_vec[i], mod);
    return x;
}

asym::zpVec asym::vector_zp_rand(int size, asym::point mod) {
    auto x = (asym::zpVec) malloc(sizeof(asym::Zp) * size);
    for (int i = 0; i < size; i++) x[i] = asym::zp_rand(mod);
    return x;
}

asym::zpVec asym::vector_join(asym::zpVec a, asym::zpVec b, int size_a, int size_b) {
    auto r = (asym::zpVec) malloc(sizeof(asym::Zp) * (size_a + size_b));
    for (int i = 0; i < size_a; i++) r[i] = asym::zp_copy(a[i]);
    for (int i = 0; i < size_b; i++) r[i + size_a] = asym::zp_copy(b[i]);
    return r;
}

asym::zpVec asym::vector_add(asym::zpVec a, asym::zpVec b, int size) {
    auto r = (asym::zpVec) malloc(sizeof(asym::Zp) * size);
    for (int i = 0; i < size; i++) r[i] = asym::zp_add(a[i], b[i]);
    return r;
}

asym::g1Vec asym::vector_raise_g1(asym::g1 base, asym::zpVec x, int size) {
    auto r = (asym::g1Vec) malloc(sizeof(asym::g1) * size);
    for (int i = 0; i < size; i++) asym::g1_mul(r[i], base, x[i]);
    return r;
}

asym::g1Vec asym::vector_raise_g1_with_table(asym::g1Vec table, asym::zpVec x, int size) {
    auto r = (asym::g1Vec) malloc(sizeof(asym::g1) * size);
    for (int i = 0; i < size; i++) g1_mul_fix(r[i], table, x[i].num);
    return r;
}

asym::g2Vec asym::vector_raise_g2(asym::g2 base, asym::zpVec x, int size) {
    auto r = (asym::g2Vec) malloc(sizeof(asym::g2) * size);
    for (int i = 0; i < size; i++) asym::g2_mul(r[i], base, x[i]);
    return r;
}

asym::g2Vec asym::vector_raise_g2_with_table(asym::g2Vec table, asym::zpVec x, int size) {
    auto r = (asym::g2Vec) malloc(sizeof(asym::g2) * size);
    for (int i = 0; i < size; i++) g2_mul_fix(r[i], table, x[i].num);
    return r;
}

void asym::inner_product(asym::gt r, asym::g1Vec a, asym::g2Vec b, int size) {
    pc_map_sim(r, a, b, size);
}