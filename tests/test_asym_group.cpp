#include "asym_group.hpp"

int test_g1_generator() {
    asym::g1 x;
    asym::g1_gen(x);
    return g1_is_valid(x) == 1;
}

int test_g2_generator() {
    asym::g2 x;
    asym::g2_gen(x);
    return g2_is_valid(x) == 1;
}

int test_gt_all() {
    // Declare variables.
    asym::g1 a;
    asym::g2 b;
    asym::gt x, y, z;

    // Set generators and get a Gt element.
    asym::g1_gen(a);
    asym::g2_gen(b);
    asym::bp_map(x, a, b);

    // Get a new gt element and compute its inverse.
    asym::gt_raise_int(y, x, 100);
    asym::gt_inverse(z, y);

    // Multiply them together and find x^0.
    asym::gt_multiply(y, y, z);
    asym::gt_get_unity(x);

    return asym::gt_compare(x, y);
}

int test_all(asym::point N) {
    // Set integers.
    asym::Zp a = asym::zp_from_int(5, N);
    asym::Zp b = asym::zp_from_int(6, N);
    asym::Zp c = asym::zp_from_int(30, N);

    // Declare variables.
    asym::g1 j, k;
    asym::g2 m, n;
    asym::gt x, y, z;

    // Set generators.
    asym::g1_gen(j);
    asym::g2_gen(m);

    // Multiply the generator by value.
    asym::g1_mul(k, j, a);
    asym::g2_mul(n, m, b);

    // Compare e(g_1^a, g_2^b) with e(g_1, g_2)^(ab).
    asym::bp_map(x, j, m);
    asym::gt_raise(z, x, c);
    asym::bp_map(y, k, n);

    return asym::gt_compare(y, z);
}

int main() {
    // Init core and setup.
    asym::point N;
    asym::init_get_order(N);

    // Perform tests.
    if (test_g1_generator() != 1) return 1;
    if (test_g2_generator() != 1) return 1;
    if (test_gt_all() != 1) return 1;
    if (test_all(N) != 1) return 1;

    return 0;
}