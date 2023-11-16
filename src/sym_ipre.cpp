#include "sym_ipre.hpp"

sym::ipre::Sk sym::ipre::setup(point secpar, int size) {
    Sk sk{};
    bn_copy(sk.mod, secpar);
    g_gen(sk.g_base);
    bp_map(sk.gt_base, sk.g_base, sk.g_base);
    sk.A = matrix_zp_rand(2, size, sk.mod);
    sk.B = matrix_zp_rand(B_SIZE, B_SIZE, sk.mod);
    sk.Bi = matrix_transpose(matrix_inverse(sk.B, B_SIZE, sk.mod), B_SIZE, B_SIZE);
    return sk;
}

sym::ipre::Ct sym::ipre::enc(Sk sk, const int *message, int size) {
    // Declare the returned ciphertext and convert message to Zp.
    Ct ct{};
    zpVec x = vector_zp_from_int(message, size, sk.mod);

    // We generate s and compute sA + x.
    zpVec s = vector_zp_rand(2, sk.mod);
    zpVec sA = matrix_multiply(s, sk.A, 1, 2, size, sk.mod);
    zpVec sAx = vector_add(sA, x, size);
    ct.ctx = vector_raise(sk.g_base, sAx, size);

    // We compute the function hiding inner product encryption ciphertext.
    zpMat AT = matrix_transpose(sk.A, 2, size);
    zpVec xAT = matrix_multiply(x, AT, 1, size, 2, sk.mod);
    zpVec xATs = vector_merge(xAT, s, 2, 2);
    zpVec xATsB = matrix_multiply(xATs, sk.B, 1, B_SIZE, B_SIZE, sk.mod);
    ct.ctc = vector_raise(sk.g_base, xATsB, B_SIZE);

    // We compute the function hiding inner product encryption derived key.
    zpVec sAAT = matrix_multiply(sA, AT, 1, size, 2, sk.mod);
    zpVec xATsAAT = vector_add(xAT, sAAT, 2);
    zpVec sxATsAAT = vector_merge(s, xATsAAT, 2, 2);
    zpVec sxATsAATBi = matrix_multiply(sxATsAAT, sk.Bi, 1, B_SIZE, B_SIZE, sk.mod);
    ct.ctk = vector_raise(sk.g_base, sxATsAATBi, B_SIZE);

    return ct;
}

int sym::ipre::eval(Sk sk, Ct x, Ct y, int size, int bound) {
    // Decrypt components.
    gt xy, ct;
    inner_product(xy, x.ctx, y.ctx, size);
    inner_product(ct, x.ctc, y.ctk, B_SIZE);

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