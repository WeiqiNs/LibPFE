#include "sym_ipre.hpp"

int test_scheme(sym::point N) {
    // Set x, y vectors.
    int x[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int y[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};
    // Initialize the scheme.
    sym::ipre::Sk key = sym::ipre::setup(N, 10);
    // Encrypt the messages.
    sym::ipre::Ct ct_x = sym::ipre::enc(key, x, 10);
    sym::ipre::Ct ct_y = sym::ipre::enc(key, y, 10);
    // Evaluate the two ciphertexts.
    int output = sym::ipre::eval(key, ct_x, ct_y, 10, 150);

    return output == 145;
}

int main() {
    // Init core and setup.
    sym::point N;
    sym::init_get_order(N);

    // Perform tests.
    if (test_scheme(N) != 1) return 1;

    return 0;
}