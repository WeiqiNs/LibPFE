#include "ipfe_kim.hpp"

IPFE::KIM::Msk IPFE::KIM::setup(const int size, const bool& pre){
    // Create the msk instance.
    Msk msk;

    // First generate the bilinear pairing group.
    msk.bpg = std::make_unique<BP>(pre);
    // Generate the random matrix of desired size.
    msk.b = msk.bpg->Zp->rand_mat(size, size);
    // Find the inverse and the determinant.
    msk.bi = msk.bpg->Zp->mat_inv_with_det(msk.b, msk.det);
    // Find its transpose multiplied with determinant.
    msk.bi = Field::mat_transpose(msk.bpg->Zp->mat_mul(msk.bi, msk.det));

    return msk;
}

IPFE::KIM::Sk IPFE::KIM::keygen(const Msk& msk, const IntVec& function){
    // First convert input function to field elements.
    auto input_vec = msk.bpg->Zp->from_int(function);

    // Sample a random point alpha.
    const auto alpha = msk.bpg->Zp->rand();

    // Compute alpha * input_vec * b.
    input_vec = msk.bpg->Zp->vec_mul(input_vec, alpha);
    input_vec = msk.bpg->Zp->mat_mul(input_vec, msk.b);

    // Create the Sk object.
    Sk sk;

    // Assign values to the secret key object.
    sk.r = msk.bpg->Gp->g2_raise(msk.bpg->Zp->mul(alpha, msk.det));
    sk.vec = msk.bpg->Gp->g2_raise(input_vec);

    return sk;
}

IPFE::KIM::Ct IPFE::KIM::enc(const Msk& msk, const IntVec& message){
    // First convert input message to field elements.
    auto input_vec = msk.bpg->Zp->from_int(message);

    // Sample a random point alpha.
    const auto beta = msk.bpg->Zp->rand();

    // Compute alpha * input_vec * bi.
    input_vec = msk.bpg->Zp->vec_mul(input_vec, beta);
    input_vec = msk.bpg->Zp->mat_mul(input_vec, msk.bi);

    // Create the Ct object.
    Ct ct;

    // Assign values to the secret key object.
    ct.r = msk.bpg->Gp->g1_raise(beta);
    ct.vec = msk.bpg->Gp->g1_raise(input_vec);

    // Raise the vector to g2 and return.
    return ct;
}

int IPFE::KIM::dec(const Sk& sk, const Ct& ct, const int lower_bound, const int upper_bound){
    // Compute the base.
    const auto base = Group::pair(ct.r, sk.r);
    // Compute the target.
    const auto target = Group::pair(ct.vec, sk.vec);
    // Find the exponent and return it.
    return Group::find_exp(base, target, lower_bound, upper_bound);
}
