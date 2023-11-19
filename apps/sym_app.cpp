#include <iostream>
#include "sym_ipre.hpp"

int main() {
    // Set x, y vectors.
    int x[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int y[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 100};

    // Initialize the scheme.
    sym::ipre::Pp ipre_pp = sym::ipre::ppgen(10, 150);
    sym::ipre::Sk ipre_sk = sym::ipre::setup(ipre_pp);
    // Encrypt the messages.
    sym::ipre::Ct ipre_ct_x = sym::ipre::enc(ipre_pp, ipre_sk, x);
    sym::ipre::Ct ipre_ct_y = sym::ipre::enc(ipre_pp, ipre_sk, y);
    // Evaluate the two ciphertexts.
    int ipre_output = sym::ipre::eval(ipre_pp, ipre_ct_x, ipre_ct_y);
    // Output message.
    if (ipre_output == 145) std::cout << "Sym scheme is working!" << std::endl;

    return 0;
}