#include "field.h"

int test_zp_zero() {
    zp x, y;
    zp_zero(x);
    fp_zero(y);
    return fp_cmp(x, y);
}

int test_zp_copy() {
    zp x, y;
    zp_from_int(x, 3);
    zp_copy(y, x);
    return fp_cmp_dig(y, 3);
}

int test_zp_from_int() {
    zp x;
    zp_from_int(x, 3);
    return fp_cmp_dig(x, 3);
}

int test_zp_add() {
    zp x, y, r;
    zp_from_int(x, 10);
    zp_from_int(y, 20);
    zp_add(r, x, y);
    return fp_cmp_dig(r, 30);
}

int test_zp_multiply() {
    zp x, y, r;
    zp_from_int(x, 10);
    zp_from_int(y, 20);
    zp_multiply(r, x, y);
    return fp_cmp_dig(r, 200);
}

int test_zp_inverse() {
    zp x, xi, r;
    rand_zp(x);
    zp_inverse(xi, x);
    zp_multiply(r, x, xi);
    return fp_cmp_dig(r, 1);
}


int main() {
    // Init core and setup.
    core_init();
    pc_param_set_any();

    // Perform tests.
    if (test_zp_zero() != RLC_EQ) return 1;
    if (test_zp_copy() != RLC_EQ) return 1;
    if (test_zp_from_int() != RLC_EQ) return 1;
    if (test_zp_add() != RLC_EQ) return 1;
    if (test_zp_multiply() != RLC_EQ) return 1;
    if (test_zp_inverse() != RLC_EQ) return 1;

    return 0;
}