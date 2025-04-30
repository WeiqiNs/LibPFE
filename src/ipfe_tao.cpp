#include "ipfe_tao.hpp"

IPFE::TAO::Msk IPFE::TAO::setup(const int size, const bool& pre){
    // Create the msk instance.
    Msk msk;

    // First generate the bilinear pairing group.
    msk.bpg = std::make_unique<BP>(pre);
    // Sample a random value.
    const Fp r = msk.bpg->Zp->rand();
    // Generate the random matrix of desired size.
    msk.b = msk.bpg->Zp->rand_mat(2 * size + 5, 2 * size + 5);
    // Find its inverse transposed and multiplied with random value.
    msk.bi = Field::mat_transpose(msk.bpg->Zp->mat_mul(msk.bpg->Zp->mat_inv(msk.b), r));
    // Compute the base in the target group.
    msk.base = std::make_unique<Gt>(msk.bpg->Gp->gt_raise(r));

    return msk;
} // LCOV_EXCL_LINE

IPFE::TAO::Sk IPFE::TAO::keygen(const Msk& msk, const IntVec& function){
    // Create a new vector of input vector and zeros.
    IntVec input_vec(function.size() * 2 + 2);
    std::copy(function.begin(), function.end(), input_vec.begin());

    // Convert the input vector to field elements.
    auto func_vec = msk.bpg->Zp->from_int(input_vec);

    // Append desired values. (two random value and a zero).
    func_vec.push_back(msk.bpg->Zp->rand());
    func_vec.push_back(msk.bpg->Zp->rand());
    func_vec.emplace_back(0);

    // Create the Sk object.
    Sk sk;

    // Assign values to the secret key object.
    sk.vec = msk.bpg->Gp->g2_raise(msk.bpg->Zp->mat_mul(func_vec, msk.b));

    return sk;
} // LCOV_EXCL_LINE

IPFE::TAO::Ct IPFE::TAO::enc(const Msk& msk, const IntVec& message){
    // Create a new vector of input vector and zeros.
    IntVec input_vec(message.size() * 2);
    std::copy(message.begin(), message.end(), input_vec.begin());

    // Convert the input vector to field elements.
    auto mess_vec = msk.bpg->Zp->from_int(input_vec);

    // Append desired values. (two random value and three zeros).
    mess_vec.push_back(msk.bpg->Zp->rand());
    mess_vec.push_back(msk.bpg->Zp->rand());
    mess_vec.emplace_back(0);
    mess_vec.emplace_back(0);
    mess_vec.emplace_back(0);

    // Create the Sk object.
    Ct ct;

    // Assign values to the secret key object.
    ct.vec = msk.bpg->Gp->g1_raise(msk.bpg->Zp->mat_mul(mess_vec, msk.bi));

    return ct;
} // LCOV_EXCL_LINE

int IPFE::TAO::dec(const Gt& base, const Sk& sk, const Ct& ct, const int lower_bound, const int upper_bound){
    // Compute the target.
    const auto target = Group::pair(ct.vec, sk.vec);
    // Find the exponent and return it.
    return Group::find_exp(base, target, lower_bound, upper_bound);
} // LCOV_EXCL_LINE
