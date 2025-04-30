#include "ipfe_opt.hpp"

// A global value for this specific scheme only.
static constexpr int B_SIZE = 4;

IPFE::OPT::Msk IPFE::OPT::setup(const int size, const bool& pre){
    // Create the msk instance.
    Msk msk;

    // First generate the bilinear pairing group.
    msk.bpg = std::make_unique<BP>(pre);
    // Generate the random rectangular matrix of desired size.
    msk.a = msk.bpg->Zp->rand_mat(2, size);
    // Generate the random matrix of desired size.
    msk.b = msk.bpg->Zp->rand_mat(B_SIZE, B_SIZE);
    // Find its inverse transposed.
    msk.bi = Field::mat_transpose(msk.bpg->Zp->mat_inv(msk.b));

    return msk;
} // LCOV_EXCL_LINE

IPFE::OPT::Sk IPFE::OPT::keygen(const Msk& msk, const IntVec& function){
    // First convert input function to field elements.
    const auto input_vec = msk.bpg->Zp->from_int(function);

    // We generate s and compute sa + f.
    const auto s = msk.bpg->Zp->rand_vec(2);
    const auto sa = msk.bpg->Zp->mat_mul(s, msk.a);
    const auto saf = msk.bpg->Zp->vec_add(sa, input_vec);

    // We compute fa^T + saa^T.
    const auto fat = msk.bpg->Zp->mat_mul(msk.a, input_vec);
    const auto aat = msk.bpg->Zp->mat_mul(msk.a, Field::mat_transpose(msk.a));
    const auto saat = msk.bpg->Zp->mat_mul(aat, s);
    const auto sum = msk.bpg->Zp->vec_add(fat, saat);

    // We compute b * (s || fa^T + saa^T).
    const auto ss = Field::vec_join(s, sum);
    const auto bss = msk.bpg->Zp->mat_mul(msk.b, ss);

    // Create the sk object.
    Sk sk;

    // Assign values to the secret key object.
    sk.vec = msk.bpg->Gp->g2_raise(saf);
    sk.r = msk.bpg->Gp->g2_raise(bss);

    return sk;
} // LCOV_EXCL_LINE

IPFE::OPT::Ct IPFE::OPT::enc(const Msk& msk, const IntVec& message){
    // First convert input function to field elements.
    const auto input_vec = msk.bpg->Zp->from_int(message);

    // We generate s and compute sa + m.
    const auto s = msk.bpg->Zp->rand_vec(2);
    const auto sa = msk.bpg->Zp->mat_mul(s, msk.a);
    const auto sam = msk.bpg->Zp->vec_add(sa, input_vec);

    // We compute bi * (am || s).
    const auto am = msk.bpg->Zp->mat_mul(msk.a, input_vec);
    const auto ams = Field::vec_join(am, s);
    const auto biams = msk.bpg->Zp->mat_mul(msk.bi, ams);

    // Create the ct object.
    Ct ct;

    // Assign values to the ciphertext object.
    ct.vec = msk.bpg->Gp->g1_raise(sam);
    ct.r = msk.bpg->Gp->g1_raise(biams);

    return ct;
} // LCOV_EXCL_LINE

int IPFE::OPT::dec(const Gt& base, const Sk& sk, const Ct& ct, const int lower_bound, const int upper_bound){
    // Compute xy + r and r in the exponent.
    const auto xyr = Group::pair(ct.vec, sk.vec);
    const auto r = Group::pair(ct.r, sk.r);
    // Compute the target.
    const auto target = Group::gt_div(xyr, r);
    // Find the exponent and return it.
    return Group::find_exp(base, target, lower_bound, upper_bound);
} // LCOV_EXCL_LINE
