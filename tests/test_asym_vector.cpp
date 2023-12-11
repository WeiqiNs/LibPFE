#include "asym_vector.hpp"

int test_zp_from_int(asym::point N) {
    int int_vec[4] = {1, 2, 3, 4};
    asym::zpVec x = asym::vector_zp_from_int(int_vec, 4, N);
    return asym::zp_cmp_int(x[3], 4);
}

int test_merge_vector(asym::point N) {
    int int_vec_x[3] = {1, 2, 3};
    int int_vec_y[3] = {11, 22, 33};
    asym::zpVec x = asym::vector_zp_from_int(int_vec_x, 3, N);
    asym::zpVec y = asym::vector_zp_from_int(int_vec_y, 3, N);
    asym::zpVec z = asym::vector_join(x, y, 3, 3);
    return asym::zp_cmp_int(z[5], 33);
}

int test_add_vector(asym::point N) {
    int int_vec_x[3] = {1, 2, 3};
    int int_vec_y[3] = {11, 22, 33};
    asym::zpVec x = asym::vector_zp_from_int(int_vec_x, 3, N);
    asym::zpVec y = asym::vector_zp_from_int(int_vec_y, 3, N);
    asym::zpVec z = asym::vector_add(x, y, 3);
    return asym::zp_cmp_int(z[2], 36);
}

int test_inner_product(asym::point N) {
    // Set vectors and value.
    int int_vec_x[3] = {1, 2, 3};
    int int_vec_y[3] = {4, 5, 6};
    asym::zpVec x = asym::vector_zp_from_int(int_vec_x, 3, N);
    asym::zpVec y = asym::vector_zp_from_int(int_vec_y, 3, N);
    asym::Zp z = asym::zp_from_int(32, N);

    // Set g_base and raise to vector power.
    asym::g1 g1_gen;
    asym::g1Vec g1x;
    asym::g1_gen(g1_gen);
    g1x = asym::vector_raise_g1(g1_gen, x, 3);

    asym::g2 g2_gen;
    asym::g2Vec g2y;
    asym::g2_gen(g2_gen);
    g2y = asym::vector_raise_g2(g2_gen, y, 3);

    // Compute inner product and check.
    asym::gt b, r;
    asym::inner_product(r, g1x, g2y, 3);
    asym::bp_map(b, g1_gen, g2_gen);
    asym::gt_raise(b, b, z);

    return asym::gt_compare(b, r);
}

int test_pre_comp_g1(asym::point N) {
    // Set vectors.
    int int_vec[5] = {1, 22, 333, 4444, 55555};
    asym::zpVec x = asym::vector_zp_from_int(int_vec, 5, N);

    // Set g_base.
    asym::g1 base;
    asym::g1_gen(base);

    // Get the precomputed table.
    asym::g1Vec gx, gy, table;
    table = asym::get_g1_pre_table(base);
    gx = asym::vector_raise_g1(base, x, 5);
    gy = asym::vector_raise_g1_with_table(table, x, 5);

    return g1_cmp(gx[3], gy[3]) == RLC_EQ;
}

int test_pre_comp_g2(asym::point N) {
    // Set vectors.
    int int_vec[5] = {1, 22, 333, 4444, 55555};
    asym::zpVec x = asym::vector_zp_from_int(int_vec, 5, N);

    // Set g_base.
    asym::g2 base;
    asym::g2_gen(base);

    // Get the precomputed table.
    asym::g2Vec gx, gy, table;
    table = asym::get_g2_pre_table(base);
    gx = asym::vector_raise_g2(base, x, 5);
    gy = asym::vector_raise_g2_with_table(table, x, 5);

    return g2_cmp(gx[3], gy[3]) == RLC_EQ;
}


int main() {
    // Init core and setup.
    asym::point N;
    asym::init_get_order(N);

    // Perform tests.
    if (test_zp_from_int(N) != 1) return 1;
    if (test_merge_vector(N) != 1) return 1;
    if (test_add_vector(N) != 1) return 1;
    if (test_inner_product(N) != 1) return 1;
    if (test_pre_comp_g1(N) != 1) return 1;
    if (test_pre_comp_g2(N) != 1) return 1;

    return 0;
}