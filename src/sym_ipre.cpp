#include "sym_ipre.hpp"

// A global value for this specific scheme only.
const static int B_SIZE = 4;

sym::ipre::Pp sym::ipre::ppgen(int size, int bound) {
    sym::ipre::Pp pp{};
    pp.size = size;
    pp.bound = bound;
    sym::init_get_order(pp.mod);
    sym::g_gen(pp.g_base);
    sym::bp_map(pp.gt_base, pp.g_base, pp.g_base);
    return pp;
}

sym::ipre::Sk sym::ipre::setup(sym::ipre::Pp pp) {
    sym::ipre::Sk sk{};
    sk.A = sym::matrix_zp_rand(2, pp.size, pp.mod);
    sk.B = sym::matrix_zp_rand(B_SIZE, B_SIZE, pp.mod);
    sk.Bi = sym::matrix_transpose(matrix_inverse(sk.B, B_SIZE, pp.mod), B_SIZE, B_SIZE);
    return sk;
}

sym::ipre::Ct sym::ipre::enc(Pp pp, Sk sk, const int *message) {
    // Declare the returned ciphertext and convert message to Zp.
    sym::ipre::Ct ct{};
    sym::zpVec x = sym::vector_zp_from_int(message, pp.size, pp.mod);

    // We generate s and compute sA + x.
    sym::zpVec s = sym::vector_zp_rand(2, pp.mod);
    sym::zpVec sA = sym::matrix_multiply(s, sk.A, 1, 2, pp.size, pp.mod);
    sym::zpVec sAx = sym::vector_add(sA, x, pp.size);
    ct.ctx = sym::vector_raise(pp.g_base, sAx, pp.size);

    // We compute the function hiding inner product encryption ciphertext.
    zpMat AT = sym::matrix_transpose(sk.A, 2, pp.size);
    sym::zpVec xAT = sym::matrix_multiply(x, AT, 1, pp.size, 2, pp.mod);
    sym::zpVec xATs = sym::vector_join(xAT, s, 2, 2);
    sym::zpVec xATsB = sym::matrix_multiply(xATs, sk.B, 1, B_SIZE, B_SIZE, pp.mod);
    ct.ctr = sym::vector_raise(pp.g_base, xATsB, B_SIZE);

    // We compute the function hiding inner product encryption derived key.
    sym::zpVec sAAT = sym::matrix_multiply(sA, AT, 1, pp.size, 2, pp.mod);
    sym::zpVec xATsAAT = sym::vector_add(xAT, sAAT, 2);
    sym::zpVec sxATsAAT = sym::vector_join(s, xATsAAT, 2, 2);
    sym::zpVec sxATsAATBi = sym::matrix_multiply(sxATsAAT, sk.Bi, 1, B_SIZE, B_SIZE, pp.mod);
    ct.ctl = sym::vector_raise(pp.g_base, sxATsAATBi, B_SIZE);

    return ct;
}

int sym::ipre::eval(Pp pp, Ct x, Ct y) {
    // Decrypt components.
    sym::gt xy, ct;
    sym::inner_product(xy, x.ctx, y.ctx, pp.size);
    sym::inner_product(ct, x.ctr, y.ctl, B_SIZE);

    // Decrypt final result.
    sym::gt_inverse(ct, ct);
    sym::gt_multiply(xy, xy, ct);

    // Get a target group element holder.
    sym::gt output;

    // Iterate through a loop to find correct answer.
    for (int i = 1; i <= pp.bound; i++) {
        sym::gt_raise_int(output, pp.gt_base, i);
        if (sym::gt_compare(output, xy)) return i;
    }

    // Otherwise return 0 as the output.
    return 0;
}

