#include "sym_group.hpp"

int test_generator() {
    sym::g x;
    sym::g_gen(x);
    return g1_is_valid(x);
}

int test_all(sym::point N) {
    // Set integers.
    sym::Zp m = sym::zp_from_int(5, N);
    sym::Zp n = sym::zp_from_int(25, N);

    // Declare variables.
    sym::g a, b;
    sym::gt x, y, z;

    // Get generator g and find g^5.
    sym::g_gen(a);
    sym::g_mul(b, a, m);

    // Get e(g, g) and e(g^5, g^5).
    sym::bp_map(x, a, a);
    sym::bp_map(y, b, b);

    // Get e(g, g)^25.
    sym::gt_raise(z, x, n);

    // Compare e(g^5, g^5) with e(g, g)^25.
    return gt_cmp(y, z) == RLC_EQ;
}

int main() {
    // Init core and setup.
    sym::point N;
    sym::init_get_order(N);

    // Perform tests.
    if (test_generator() != 1) return 1;
    if (test_all(N) != 1) return 1;

    return 0;
}