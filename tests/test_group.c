#include "group.h"

int test_generator() {
    g x;
    gen(x);
    return g1_is_valid(x);
}

int test_all(bn_st *N) {
    // Set integers.
    struct zp m = zp_from_int(5, N);
    struct zp n = zp_from_int(25, N);

    // Declare variables.
    g a, b;
    gt x, y, z;

    // Get generator g and find g^5.
    gen(a);
    g_mul(b, a, m);

    // Get e(g, g) and e(g^5, g^5).
    bp_map(a, a, x);
    bp_map(b, b, y);

    // Get e(g, g)^25.
    gt_raise(z, x, n);

    // Compare e(g^5, g^5) with e(g, g)^25.
    return gt_cmp(y, z);
}

int main() {
    // Init core and setup.
    core_init();
    pc_param_set_any();

    // Get order.
    bn_t N;
    pc_get_ord(N);

    // Perform tests.
    if (test_generator() != 1) return 1;
    if (test_all(N) != RLC_EQ) return 1;

    return 0;
}