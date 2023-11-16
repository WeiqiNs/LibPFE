#include "sym_init.hpp"

int test_sym_init() {
    sym::point N;
    sym::init_get_order(N);
    // The group order should be a prime.
    return bn_is_prime(N) == 1;
}


int main() {
    // Perform tests.
    if (test_sym_init() != 1) return 1;

    return 0;
}