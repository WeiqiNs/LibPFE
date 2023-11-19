#include "asym_ipfe_kim.hpp"

int test_scheme() {
    // Set x, y vectors.
    int x[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int y[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};
    // Initialize the scheme.
    asym::ipfe::kim::Pp pp = asym::ipfe::kim::ppgen(10, 150);
    asym::ipfe::kim::Sk sk = asym::ipfe::kim::setup(pp);
    // Encrypt the messages.
    asym::ipfe::kim::Key key_y = asym::ipfe::kim::keyGen(pp, sk, y);
    asym::ipfe::kim::Ct ct_x = asym::ipfe::kim::enc(pp, sk, x);
    // Evaluate the two ciphertexts.
    int output = asym::ipfe::kim::dec(pp, key_y, ct_x);

    return output == 145;
}

int main() {
    // Perform tests.
    if (test_scheme() != 1) return 1;

    return 0;
}