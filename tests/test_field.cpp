#include "field.h"

int test_zp_zero(bn_st *N) {
    ZP x = zp_zero(N);
    return zp_cmp_int(x, 0);
}

int test_zp_one(bn_st *N) {
    ZP x = zp_one(N);
    return zp_cmp_int(x, 1);
}

int test_zp_copy(bn_st *N) {
    ZP x = zp_from_int(10, N);
    ZP y = zp_copy(x);
    return zp_cmp(x, y);
}

int test_zp_from_int(bn_st *N) {
    ZP x = zp_from_int(3, N);
    return zp_cmp_int(x, 3);
}

int test_zp_add(bn_st *N) {
    ZP x = zp_from_int(10, N);
    ZP y = zp_from_int(20, N);
    ZP z = zp_add(x, y);
    return zp_cmp_int(z, 30);
}

int test_zp_neg(bn_st *N) {
    ZP x = rand_zp(N);
    ZP y = zp_neg(x);
    ZP z = zp_add(x, y);
    return zp_cmp_int(z, 0);
}

int test_zp_mul(bn_st *N) {
    ZP x = zp_from_int(10, N);
    ZP y = zp_from_int(20, N);
    ZP z = zp_mul(x, y);
    return zp_cmp_int(z, 200);
}

int test_zp_inv(bn_st *N) {
    ZP x = rand_zp(N);
    ZP y = zp_inv(x);
    ZP z = zp_mul(x, y);
    return zp_cmp_int(z, 1);
}

int main() {
    // Init core and setup.
    core_init();
    pc_param_set_any();

    // Get order.
    bn_t N;
    pc_get_ord(N);

    // Perform tests.
    if (test_zp_zero(N) != 1) return 1;
    if (test_zp_one(N) != 1) return 1;
    if (test_zp_copy(N) != 1) return 1;
    if (test_zp_from_int(N) != 1) return 1;
    if (test_zp_add(N) != 1) return 1;
    if (test_zp_neg(N) != 1) return 1;
    if (test_zp_mul(N) != 1) return 1;
    if (test_zp_inv(N) != 1) return 1;

    return 0;
}