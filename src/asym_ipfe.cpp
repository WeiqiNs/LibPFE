#include "asym_ipfe.hpp"

asym::ipfe::Sk asym::ipfe::setup(point secpar, int size) {
    Sk sk{};
    bn_copy(sk.mod, secpar);
    g1_gen(sk.g1_base);
    g2_gen(sk.g2_base);
    bp_map(sk.gt_base, sk.g1_base, sk.g2_base);
    sk.A = matrix_zp_rand(2, size, sk.mod);
    sk.B = matrix_zp_rand(B_SIZE, B_SIZE, sk.mod);
    sk.Bi = matrix_transpose(matrix_inverse(sk.B, B_SIZE, sk.mod), B_SIZE, B_SIZE);
    return sk;
}

asym::ipfe::Key asym::ipfe::keyGen(Sk sk, const int *function, int size) {
    // Declare the returned key and convert message to Zp.
    Key key{};
    zpVec y = vector_zp_from_int(function, size, sk.mod);

    // We generate s and compute sA + x.
    zpVec s = vector_zp_rand(2, sk.mod);
    zpVec sA = matrix_multiply(s, sk.A, 1, 2, size, sk.mod);
    zpVec sAx = vector_add(sA, y, size);
    key.ctx = vector_raise_g1(sk.g1_base, sAx, size);

    // We compute the function hiding inner product encryption derived key.
    zpMat AT = matrix_transpose(sk.A, 2, size);
    zpVec xAT = matrix_multiply(y, AT, 1, size, 2, sk.mod);
    zpVec sAAT = matrix_multiply(sA, AT, 1, size, 2, sk.mod);
    zpVec xATsAAT = vector_add(xAT, sAAT, 2);
    zpVec sxATsAAT = vector_merge(s, xATsAAT, 2, 2);
    zpVec sxATsAATBi = matrix_multiply(sxATsAAT, sk.Bi, 1, B_SIZE, B_SIZE, sk.mod);
    key.ctk = vector_raise_g1(sk.g1_base, sxATsAATBi, B_SIZE);

    return key;
}

asym::ipfe::Ct asym::ipfe::enc(Sk sk, const int *message, int size) {
    // Declare the returned ciphertext and convert message to Zp.
    Ct ct{};
    zpVec x = vector_zp_from_int(message, size, sk.mod);

    // We generate s and compute sA + x.
    zpVec s = vector_zp_rand(2, sk.mod);
    zpVec sA = matrix_multiply(s, sk.A, 1, 2, size, sk.mod);
    zpVec sAx = vector_add(sA, x, size);
    ct.ctx = vector_raise_g2(sk.g2_base, sAx, size);

    // We compute the function hiding inner product encryption ciphertext.
    zpMat AT = matrix_transpose(sk.A, 2, size);
    zpVec xAT = matrix_multiply(x, AT, 1, size, 2, sk.mod);
    zpVec xATs = vector_merge(xAT, s, 2, 2);
    zpVec xATsB = matrix_multiply(xATs, sk.B, 1, B_SIZE, B_SIZE, sk.mod);
    ct.ctc = vector_raise_g2(sk.g2_base, xATsB, B_SIZE);

    return ct;
}

int asym::ipfe::dec(Sk sk, Key y, Ct x, int size, int bound) {
    // Decrypt components.
    gt xy, ct;
    inner_product(xy, y.ctx, x.ctx, size);
    inner_product(ct, y.ctk, x.ctc, B_SIZE);

    // Decrypt final result.
    gt_inv(ct, ct);
    gt_mul(xy, xy, ct);

    // Get a target group element holder.
    gt output;

    // Iterate through a loop to find correct answer.
    for (int i = 1; i <= bound; i++) {
        gt_exp_dig(output, sk.gt_base, i);
        if (gt_cmp(output, xy) == RLC_EQ) return i;
    }

    // Otherwise return 0 as the output.
    return 0;
}
