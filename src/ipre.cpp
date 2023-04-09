#include "ipre.h"

key setup(int size) {
    key key{};
    pc_get_ord(key.modular);
    gen(key.base);
    bp_map(key.base, key.base, key.t_base);
    key.A = matrix_zp_rand(2, size, key.modular);
    key.B = matrix_zp_rand(B_SIZE, B_SIZE, key.modular);
    key.Bi = matrix_transpose(matrix_inverse(key.B, B_SIZE, key.modular), B_SIZE, B_SIZE);
    return key;
}

ct enc(key key, const int *message, int size) {
    // Declare the returned ciphertext and convert message to zp.
    ct ct{};
    zp_vec x = vector_zp_from_int(message, size, key.modular);

    // Helper values.
    int one[] = {1}, zero[] = {0};
    zp_vec one_vec = vector_zp_from_int(one, 1, key.modular);
    zp_vec zero_vec = vector_zp_from_int(zero, 1, key.modular);

    // We generate s and compute sA + x.
    zp_vec s = vector_zp_rand(2, key.modular);
    zp_vec sA = matrix_multiply(s, key.A, 1, 2, size, key.modular);
    zp_vec sAx = vector_add(sA, x, size);
    ct.ctx = vector_raise(key.base, sAx, size);

    // We compute the function hiding inner product encryption key.
    zp_mat AT = matrix_transpose(key.A, 2, size);
    zp_vec xAT = matrix_multiply(x, AT, 1, size, 2, key.modular);
    zp_vec xATs = vector_merge(xAT, s, 2, 2);
    zp_vec xATs0 = vector_merge(xATs, zero_vec, 4, 1);
    zp_vec xATs01 = vector_merge(xATs0, one_vec, 5, 1);
    zp_vec xATs01_B = matrix_multiply(xATs01, key.B, 1, B_SIZE, B_SIZE, key.modular);
    ct.ctc = vector_raise(key.base, xATs01_B, B_SIZE);

    // We compute the function hiding inner product encryption ciphertext.
    zp_vec sAAT = matrix_multiply(sA, AT, 1, size, 2, key.modular);
    zp_vec xATsAAT = vector_add(xAT, sAAT, 2);
    zp_vec sxATsAAT = vector_merge(s, xATsAAT, 2, 2);
    zp_vec sxATsAAT1 = vector_merge(sxATsAAT, one_vec, 4, 1);
    zp_vec sxATsAAT10 = vector_merge(sxATsAAT1, zero_vec, 5, 1);
    zp_vec sxATsAAT10_Bi = matrix_multiply(sxATsAAT10, key.Bi, 1, B_SIZE, B_SIZE, key.modular);
    ct.ctk = vector_raise(key.base, sxATsAAT10_Bi, B_SIZE);

    return ct;
}

int eval(key key, ct x, ct y, int size, int bound) {
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
        gt_exp_dig(output, key.t_base, i);
        if (gt_cmp(output, xy) == RLC_EQ) return i;
    }

    // Otherwise return 0 as the output.
    return 0;
}