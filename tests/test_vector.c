#include "vector.h"

int test_zp_from_int() {
    zp x[4];
    int int_vec[4] = {1, 2, 3, 4};
    vector_zp_from_int(x, int_vec, 4);
    return fp_cmp_dig(x[3], 4);
}

int test_merge_vector() {
    zp x[3], y[3], z[6];
    int int_vec_x[3] = {1, 2, 3};
    int int_vec_y[3] = {11, 22, 33};
    vector_zp_from_int(x, int_vec_x, 3);
    vector_zp_from_int(y, int_vec_y, 3);
    vector_merge(z, x, y, 3, 3);
    return fp_cmp_dig(z[5], 33);
}

int test_add_vector() {
    zp x[3], y[3], z[3];
    int int_vec_x[3] = {1, 2, 3};
    int int_vec_y[3] = {11, 22, 33};
    vector_zp_from_int(x, int_vec_x, 3);
    vector_zp_from_int(y, int_vec_y, 3);
    vector_add(z, x, y, 3);
    return fp_cmp_dig(z[2], 36);
}

int test_inner_product() {
    zp x[3], y[3];
    int int_vec_x[3] = {1, 2, 3};
    int int_vec_y[3] = {4, 5, 6};
    vector_zp_from_int(x, int_vec_x, 3);
    vector_zp_from_int(y, int_vec_y, 3);

    g g, gx[3], gy[3];
    generator(g);
    vector_raise(g, x, gx, 3);
    vector_raise(g, y, gy, 3);

    gt b, r;
    inner_product(r, gx, gy, 3);
    map(g, g, b);
    gt_exp_dig(b, b, 32);

    return gt_cmp(b, r);
}


int main() {
    // Init core and setup.
    core_init();
    pc_param_set_any();

    // Perform tests.
    if (test_zp_from_int() != RLC_EQ) return 1;
    if (test_merge_vector() != RLC_EQ) return 1;
    if (test_add_vector() != RLC_EQ) return 1;
    if (test_inner_product() != RLC_EQ) return 1;

    return 0;
}