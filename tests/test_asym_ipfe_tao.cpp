#include "asym_ipfe_tao.hpp"

int test_scheme() {
    // Set x, y vectors.
    int x[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int y[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};
    // Initialize the scheme.
    asym::ipfe::tao::Pp pp = asym::ipfe::tao::ppgen(10, 150);
    asym::ipfe::tao::Sk sk = asym::ipfe::tao::setup(pp);
    // Encrypt the messages.
    asym::ipfe::tao::Key key_y = asym::ipfe::tao::keyGen(pp, sk, y);
    asym::ipfe::tao::Ct ct_x = asym::ipfe::tao::enc(pp, sk, x);
    // Evaluate the two ciphertexts.
    int output = asym::ipfe::tao::dec(pp, key_y, ct_x);

    return output == 145;
}

int main() {
    // Perform tests.
    if (test_scheme() != 1) return 1;

    return 0;
}