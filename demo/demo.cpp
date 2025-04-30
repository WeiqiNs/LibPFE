#include <iostream>
#include <RIPFE/ipfe_opt.hpp>

int main(){
    std::cout << "Testing import the RIPFE library..." << std::endl;

    // Generate the master secret key.
    const auto msk = IPFE::OPT::setup(10);
    // One could assign the base to another variable (treat it as public parameter).
    const auto base = msk.bpg->Gp->get_gt();

    // Set the testing integer vectors.
    const IntVec x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const IntVec y = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Generate ciphertext and function key.
    const auto sk = IPFE::OPT::keygen(msk, x);
    const auto ct = IPFE::OPT::enc(msk, y);

    // Compute the result.
    const auto r = IPFE::OPT::dec(base, sk, ct, 300, 400);

    // Perform testing.
    if (r == 330){
        std::cout << "IPFE computation succeeded!" << std::endl;
    } else{
        std::cout << "IPFE computation failed!" << std::endl;
    }

    // Close the group.
    BP::close();

    return 0;
}
