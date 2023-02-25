#include "group.h"

int test_generator() {
    g x;
    generator(x);
    return g1_is_valid(x);
}

int test_all() {
    // Set integers.
    zp m, n;
    zp_from_int(m, 5);
    zp_from_int(n, 25);

    // Declare variables.
    g a, b;
    gt x, y, z;

    // Get generator g and find g^5.
    generator(a);
    multiply(b, a, m);

    // Get e(g, g) and e(g^5, g^5).
    map(a, a, x);
    map(b, b, y);

    // Get e(g, g)^25.
    exponentiation(z, x, n);

    // Compare e(g^5, g^5) with e(g, g)^25.
    return gt_cmp(y, z);
}

int main(){
    // Init core and setup.
    core_init();
    fp_prime_init();
    pc_param_set_any();

    // Perform tests.
    if (test_generator() != 1) return 1;
    if (test_all() != RLC_EQ) return 1;

    return 0;
}