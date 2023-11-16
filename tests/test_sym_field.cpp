#include "sym_field.hpp"

int test_zp_zero(sym::point N) {
    sym::Zp x = sym::zp_zero(N);
    return sym::zp_cmp_int(x, 0);
}

int test_zp_one(sym::point N) {
    sym::Zp x = sym::zp_one(N);
    return sym::zp_cmp_int(x, 1);
}

int test_zp_copy(sym::point N) {
    sym::Zp x = sym::zp_from_int(10, N);
    sym::Zp y = sym::zp_copy(x);
    return sym::zp_cmp(x, y);
}

int test_zp_from_int(sym::point N) {
    sym::Zp x = sym::zp_from_int(3, N);
    return sym::zp_cmp_int(x, 3);
}

int test_zp_add(sym::point N) {
    sym::Zp x = sym::zp_from_int(10, N);
    sym::Zp y = sym::zp_from_int(20, N);
    sym::Zp z = sym::zp_add(x, y);
    return sym::zp_cmp_int(z, 30);
}

int test_zp_neg(sym::point N) {
    sym::Zp x = sym::zp_rand(N);
    sym::Zp y = sym::zp_neg(x);
    sym::Zp z = sym::zp_add(x, y);
    return sym::zp_cmp_int(z, 0);
}

int test_zp_mul(sym::point N) {
    sym::Zp x = sym::zp_from_int(10, N);
    sym::Zp y = sym::zp_from_int(20, N);
    sym::Zp z = sym::zp_mul(x, y);
    return sym::zp_cmp_int(z, 200);
}

int test_zp_inv(sym::point N) {
    sym::Zp x = sym::zp_rand(N);
    sym::Zp y = sym::zp_inv(x);
    sym::Zp z = sym::zp_mul(x, y);
    return sym::zp_cmp_int(z, 1);
}

int main() {
    // Init core and setup.
    sym::point N;
    sym::init_get_order(N);

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