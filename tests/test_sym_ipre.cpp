#include "sym_ipre.hpp"

int test_scheme() {
    // Set x, y vectors.
    int x[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int y[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};

    // Initialize the scheme.
    sym::ipre::Pp pp = sym::ipre::ppgen(10, 150);
    sym::ipre::Sk sk = sym::ipre::setup(pp);
    // Encrypt the messages.
    sym::ipre::Ct ct_x = sym::ipre::enc(pp, sk, x);
    sym::ipre::Ct ct_y = sym::ipre::enc(pp, sk, y);
    // Evaluate the two ciphertexts.
    int output = sym::ipre::eval(pp, ct_x, ct_y);

    return output == 145;
}

int main() {
    // Perform tests.
    if (test_scheme() != 1) return 1;

    return 0;
}