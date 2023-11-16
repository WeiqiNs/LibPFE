#include "asym_ipfe.hpp"

int test_scheme(asym::point N) {
    // Set x, y vectors.
    int x[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int y[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};
    // Initialize the scheme.
    asym::ipfe::Sk key = asym::ipfe::setup(N, 10);
    // Encrypt the messages.
    asym::ipfe::Key ct_y = asym::ipfe::keyGen(key, y, 10);
    asym::ipfe::Ct ct_x = asym::ipfe::enc(key, x, 10);
    // Evaluate the two ciphertexts.
    int output = asym::ipfe::dec(key, ct_y, ct_x, 10, 150);

    return output == 145;
}

int main() {
    // Init core and setup.
    asym::point N;
    asym::init_get_order(N);

    // Perform tests.
    if (test_scheme(N) != 1) return 1;

    return 0;
}