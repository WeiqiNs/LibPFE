#include "sym_vector.hpp"

int test_zp_from_int(sym::point N) {
    int int_vec[4] = {1, 2, 3, 4};
    sym::zpVec x = sym::vector_zp_from_int(int_vec, 4, N);
    return sym::zp_cmp_int(x[3], 4);
}

int test_merge_vector(sym::point N) {
    int int_vec_x[3] = {1, 2, 3};
    int int_vec_y[3] = {11, 22, 33};
    sym::zpVec x = sym::vector_zp_from_int(int_vec_x, 3, N);
    sym::zpVec y = sym::vector_zp_from_int(int_vec_y, 3, N);
    sym::zpVec z = sym::vector_join(x, y, 3, 3);
    return sym::zp_cmp_int(z[5], 33);
}

int test_add_vector(sym::point N) {
    int int_vec_x[3] = {1, 2, 3};
    int int_vec_y[3] = {11, 22, 33};
    sym::zpVec x = sym::vector_zp_from_int(int_vec_x, 3, N);
    sym::zpVec y = sym::vector_zp_from_int(int_vec_y, 3, N);
    sym::zpVec z = sym::vector_add(x, y, 3);
    return sym::zp_cmp_int(z[2], 36);
}

int test_inner_product(sym::point N) {
    // Set vectors.
    int int_vec_x[3] = {1, 2, 3};
    int int_vec_y[3] = {4, 5, 6};
    sym::zpVec x = sym::vector_zp_from_int(int_vec_x, 3, N);
    sym::zpVec y = sym::vector_zp_from_int(int_vec_y, 3, N);
    sym::Zp z = sym::zp_from_int(32, N);

    // Set g_base and raise to vector power.
    sym::g base;
    sym::gVec gx, gy;
    sym::g_gen(base);
    gx = sym::vector_raise(base, x, 3);
    gy = sym::vector_raise(base, y, 3);

    // Compute inner product and check.
    sym::gt b, r;
    sym::inner_product(r, gx, gy, 3);
    sym::bp_map(b, base, base);
    sym::gt_raise(b, b, z);

    return sym::gt_compare(b, r);
}

int test_pre_comp(sym::point N) {
    // Set vectors.
    int int_vec[5] = {1, 22, 333, 4444, 55555};
    sym::zpVec x = sym::vector_zp_from_int(int_vec, 5, N);

    // Set g_base.
    sym::g base;
    sym::g_gen(base);

    // Get the precomputed table.
    sym::gVec gx, gy, table;
    table = sym::get_g_pre_table(base);
    gx = sym::vector_raise(base, x, 5);
    gy = sym::vector_raise_with_table(table, x, 5);

    return g1_cmp(gx[3], gy[3]) == RLC_EQ;
}

int main() {
    // Init core and setup.
    sym::point N;
    sym::init_get_order(N);

    // Perform tests.
    if (test_zp_from_int(N) != 1) return 1;
    if (test_merge_vector(N) != 1) return 1;
    if (test_add_vector(N) != 1) return 1;
    if (test_inner_product(N) != 1) return 1;
    if (test_pre_comp(N) != 1) return 1;

    return 0;
}