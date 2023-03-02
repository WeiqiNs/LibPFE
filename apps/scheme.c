#include "vector.h"
#include "matrix.h"

const int B_SIZE = 6;

struct key {
    zp_mat A;
    zp_mat B;
    zp_mat Bi;
    g base;
    gt t_base;
    bn_t modular;
};

struct ct {
    g_vec ctx;
    g_vec ctk;
    g_vec ctc;
};

void initialize_relic() {
    core_init();
    pc_param_set_any();
}

struct key setup(int size) {
    struct key key;
    pc_get_ord(key.modular);
    gen(key.base);
    bp_map(key.base, key.base, key.t_base);
    key.A = matrix_zp_rand(2, size, key.modular);
    key.B = matrix_zp_rand(B_SIZE, B_SIZE, key.modular);
    key.Bi = matrix_transpose(matrix_inverse(key.B, B_SIZE, key.modular), B_SIZE, B_SIZE);
    return key;
}

struct ct enc(struct key key, const int *message, int size) {
    // Declare the returned ciphertext and convert message to zp.
    struct ct ct;
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

void eval(struct key key, struct ct x, struct ct y, int size) {
    // Decrypt components.
    gt xy, ct;
    inner_product(xy, x.ctx, y.ctx, size);
    inner_product(ct, x.ctc, y.ctk, B_SIZE);

    // Decrypt final result.
    gt_inv(ct, ct);
    gt_mul(xy, xy, ct);

    // Check correctness.
    gt desired_output;
    gt_exp_dig(desired_output, key.t_base, 65);
    if (gt_cmp(desired_output, xy) == RLC_EQ) printf("Magic happened");
}

int main() {
    // Initialize relic.
    initialize_relic();
    // Set x, y vectors.
    int x[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 2};
    int y[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // Initialize the scheme.
    struct key key = setup(10);
    // Encrypt the messages.
    struct ct ct_x = enc(key, x, 10);
    struct ct ct_y = enc(key, y, 10);
    // Evaluate the two ciphertexts.
    eval(key, ct_x, ct_y, 10);
    return 0;
}