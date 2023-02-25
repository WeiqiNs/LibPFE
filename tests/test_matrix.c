#include "matrix.h"

int test_zp_from_int() {
    int row = 3, col = 3;
    zp x[row * col];
    int int_vec[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    matrix_zp_from_int(x, int_vec, 3, 3);
    return fp_cmp_dig(x[2 * col + 2], 9);
}

int test_transpose() {
    int row = 3, col = 3;
    zp x[row * col], xt[col * row];
    matrix_zp_rand(x, row, col);
    matrix_transpose(xt, x, row, col);
    return fp_cmp(xt[col - 1], x[2 * row]);
}

int test_identity() {
    int size = 1000;
    zp *x;
    x = (zp *) malloc(size * size * sizeof(zp));
    matrix_identity(x, size);
    return matrix_is_identity(x, size);
}

int test_merge() {
    int size = 10;
    zp xy[(size + size) * size], x[size * size], y[size * size];
    matrix_zp_rand(x, size, size);
    matrix_identity(y, size);
    matrix_merge(xy, x, y, size, size, size);
    return fp_cmp(x[2 * size + 1], xy[4 * size + 1]);
}

int test_multiply_vector() {
    int int_x[5] = {1, 2, 3, 4, 5};
    int int_y[15] = {10, 20, 30,
                     10, 20, 30,
                     10, 20, 30,
                     10, 20, 30,
                     10, 20, 30};

    zp x[5], y[15];
    matrix_zp_from_int(x, int_x, 1, 5);
    matrix_zp_from_int(y, int_y, 5, 3);

    zp xy[3];
    matrix_multiply(xy, x, y, 1, 5, 3);

    return fp_cmp_dig(xy[1], 300);
}

int test_inverse() {
    int size = 100;
    // Allocate space.
    zp *x, *xi, *r;
    x = (zp *) malloc(size * size * sizeof(zp));
    xi = (zp *) malloc(size * size * sizeof(zp));
    r = (zp *) malloc(size * size * sizeof(zp));
    matrix_zp_rand(x, size, size);
    matrix_inverse(xi, x, size);
    matrix_multiply(r, xi, x, size, size, size);
    return matrix_is_identity(r, size);
}


int main() {
    // Init core and setup.
    core_init();
    pc_param_set_any();

    // Perform tests.
    if (test_zp_from_int() != RLC_EQ) return 1;
    if (test_transpose() != RLC_EQ) return 1;
    if (test_identity() != 1) return 1;
    if (test_merge() != RLC_EQ) return 1;
    if (test_multiply_vector() != RLC_EQ) return 1;
    if (test_inverse() != 1) return 1;

    return 0;
}