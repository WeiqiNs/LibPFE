#include "asym_init.hpp"

int test_asym_init() {
    asym::point N;
    asym::init_get_order(N);
    // The group order should be a prime.
    return bn_is_prime(N) == 1;
}


int main() {
    // Perform tests.
    if (test_asym_init() != 1) return 1;

    return 0;
}