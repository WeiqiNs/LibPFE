#include "asym_ipfe.hpp"

// A global value for this specific scheme only.
const static int B_SIZE = 4;

asym::ipfe::Pp asym::ipfe::ppgen(int size, int bound) {
    asym::ipfe::Pp pp{};
    pp.size = size;
    pp.bound = bound;
    asym::init_get_order(pp.mod);
    asym::g1_gen(pp.g1_base);
    asym::g2_gen(pp.g2_base);
    asym::bp_map(pp.gt_base, pp.g1_base, pp.g2_base);
    return pp;
}

asym::ipfe::Sk asym::ipfe::setup(asym::ipfe::Pp pp) {
    asym::ipfe::Sk sk{};
    sk.A = asym::matrix_zp_rand(2, pp.size, pp.mod);
    sk.B = asym::matrix_zp_rand(B_SIZE, B_SIZE, pp.mod);
    sk.Bi = asym::matrix_transpose(asym::matrix_inverse(sk.B, B_SIZE, pp.mod), B_SIZE, B_SIZE);
    return sk;
}

asym::ipfe::Key asym::ipfe::keyGen(asym::ipfe::Pp pp, asym::ipfe::Sk sk, const int *function) {
    // Declare the returned key and convert message to Zp.
    asym::ipfe::Key key{};
    asym::zpVec y = asym::vector_zp_from_int(function, pp.size, pp.mod);

    // We generate s and compute sA + x.
    asym::zpVec s = asym::vector_zp_rand(2, pp.mod);
    asym::zpVec sA = asym::matrix_multiply(s, sk.A, 1, 2, pp.size, pp.mod);
    asym::zpVec sAx = asym::vector_add(sA, y, pp.size);
    key.ct = asym::vector_raise_g1(pp.g1_base, sAx, pp.size);

    // We compute the function hiding inner product encryption derived key.
    asym::zpMat AT = asym::matrix_transpose(sk.A, 2, pp.size);
    asym::zpVec xAT = asym::matrix_multiply(y, AT, 1, pp.size, 2, pp.mod);
    asym::zpVec sAAT = asym::matrix_multiply(sA, AT, 1, pp.size, 2, pp.mod);
    asym::zpVec xATsAAT = asym::vector_add(xAT, sAAT, 2);
    asym::zpVec sxATsAAT = asym::vector_join(s, xATsAAT, 2, 2);
    asym::zpVec sxATsAATBi = asym::matrix_multiply(sxATsAAT, sk.Bi, 1, B_SIZE, B_SIZE, pp.mod);
    key.ctl = asym::vector_raise_g1(pp.g1_base, sxATsAATBi, B_SIZE);

    return key;
}

asym::ipfe::Ct asym::ipfe::enc(asym::ipfe::Pp pp, asym::ipfe::Sk sk, const int *message) {
    // Declare the returned ciphertext and convert message to Zp.
    asym::ipfe::Ct ct{};
    asym::zpVec x = asym::vector_zp_from_int(message, pp.size, pp.mod);

    // We generate s and compute sA + x.
    asym::zpVec s = asym::vector_zp_rand(2, pp.mod);
    asym::zpVec sA = asym::matrix_multiply(s, sk.A, 1, 2, pp.size, pp.mod);
    asym::zpVec sAx = asym::vector_add(sA, x, pp.size);
    ct.ct = asym::vector_raise_g2(pp.g2_base, sAx, pp.size);

    // We compute the function hiding inner product encryption ciphertext.
    asym::zpMat AT = asym::matrix_transpose(sk.A, 2, pp.size);
    asym::zpVec xAT = asym::matrix_multiply(x, AT, 1, pp.size, 2, pp.mod);
    asym::zpVec xATs = asym::vector_join(xAT, s, 2, 2);
    asym::zpVec xATsB = asym::matrix_multiply(xATs, sk.B, 1, B_SIZE, B_SIZE, pp.mod);
    ct.ctr = asym::vector_raise_g2(pp.g2_base, xATsB, B_SIZE);

    return ct;
}

int asym::ipfe::dec(Pp pp, Key y, Ct x) {
    // Decrypt components.
    asym::gt xy, ct;
    asym::inner_product(xy, y.ct, x.ct, pp.size);
    asym::inner_product(ct, y.ctl, x.ctr, B_SIZE);

    // Decrypt final result.
    asym::gt_inverse(ct, ct);
    asym::gt_multiply(xy, xy, ct);

    // Get a target group element holder.
    asym::gt output;

    // Iterate through a loop to find correct answer.
    for (int i = 1; i <= pp.bound; i++) {
        asym::gt_raise_int(output, pp.gt_base, i);
        if (asym::gt_compare(output, xy)) return i;
    }

    // Otherwise return 0 as the output.
    return 0;
}

