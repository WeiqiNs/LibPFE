#include "ipre.h"

int test_scheme() {
    // Set x, y vectors.
    int x[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int y[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1000};
    // Initialize the scheme.
    key key = setup(10);
    // Encrypt the messages.
    ct ct_x = enc(key, x, 10);
    ct ct_y = enc(key, y, 10);
    // Evaluate the two ciphertexts.
    int output = eval(key, ct_x, ct_y, 10, 5000);

    return output == 1045;
}

int main() {
    // Init core and setup.
    core_init();
    pc_param_set_any();

    // Perform tests.
    if (test_scheme() != 1) return 1;

    return 0;
}