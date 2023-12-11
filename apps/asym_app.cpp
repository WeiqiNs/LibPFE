#include <iostream>
#include "asym_ipfe.hpp"

int main() {
    // Set x, y vectors.
    int x[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int y[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};

    // Initialize the scheme.
    asym::ipfe::Pp ipfe_pp = asym::ipfe::ppgen(true, 10, 150);
    asym::ipfe::Sk ipfe_sk = asym::ipfe::setup(ipfe_pp);
    // Encrypt the messages.
    asym::ipfe::Key ipfe_key_y = asym::ipfe::keyGen(ipfe_pp, ipfe_sk, y);
    asym::ipfe::Ct ipfe_ct_x = asym::ipfe::enc(ipfe_pp, ipfe_sk, x);
    // Evaluate the two ciphertexts.
    int ipfe_output = asym::ipfe::dec(ipfe_pp, ipfe_key_y, ipfe_ct_x);
    if (ipfe_output == 145) std::cout << "Asym scheme is working!" << std::endl;

    return 0;
}