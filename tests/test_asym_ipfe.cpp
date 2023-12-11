#include "asym_ipfe.hpp"

int test_scheme() {
    // Set x, y vectors.
    int x[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int y[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};
    // Initialize the scheme.
    asym::ipfe::Pp pp = asym::ipfe::ppgen(true, 10, 150);
    asym::ipfe::Sk sk = asym::ipfe::setup(pp);
    // Encrypt the messages.
    asym::ipfe::Key key_y = asym::ipfe::keyGen(pp, sk, y);
    asym::ipfe::Ct ct_x = asym::ipfe::enc(pp, sk, x);
    // Evaluate the two ciphertexts.
    int output = asym::ipfe::dec(pp, key_y, ct_x);

    return output == 145;
}

int main() {
    // Perform tests.
    if (test_scheme() != 1) return 1;

    return 0;
}