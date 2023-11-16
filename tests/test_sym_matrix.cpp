#include "sym_matrix.hpp"

int test_zp_from_int(sym::point N) {
    int int_mat[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    sym::zpMat x = sym::matrix_zp_from_int(int_mat, 3, 3, N);
    return sym::zp_cmp_int(x[8], 9);
}

int test_transpose(sym::point N) {
    int row = 3, col = 3;
    sym::zpMat x = sym::matrix_zp_rand(row, col, N);
    sym::zpMat xt = sym::matrix_transpose(x, row, col);
    return sym::zp_cmp(xt[col - 1], x[2 * row]);
}

int test_identity(sym::point N) {
    int size = 10;
    sym::zpMat x = sym::matrix_identity(size, N);
    return sym::matrix_is_identity(x, size);
}

int test_merge(sym::point N) {
    int size = 10;
    sym::zpMat x = sym::matrix_zp_rand(size, size, N);
    sym::zpMat y = sym::matrix_identity(size, N);
    sym::zpMat xy = sym::matrix_merge(x, y, size, size, size);
    return sym::zp_cmp(x[2 * size + 1], xy[4 * size + 1]);
}

int test_multiply_vector(sym::point N) {
    int mat_x[5] = {1, 2, 3, 4, 5};
    int mat_y[15] = {
            10, 20, 30,
            10, 20, 30,
            10, 20, 30,
            10, 20, 30,
            10, 20, 30
    };

    sym::zpMat x = sym::matrix_zp_from_int(mat_x, 1, 5, N);
    sym::zpMat y = sym::matrix_zp_from_int(mat_y, 5, 3, N);
    sym::zpMat xy = sym::matrix_multiply(x, y, 1, 5, 3, N);
    return sym::zp_cmp_int(xy[2], 450);
}

int test_inverse(sym::point N) {
    int size = 10;
    sym::zpMat x = sym::matrix_zp_rand(size, size, N);
    sym::zpMat xi = sym::matrix_inverse(x, size, N);
    sym::zpMat r = sym::matrix_multiply(x, xi, size, size, size, N);
    return sym::matrix_is_identity(r, size);
}

int main() {
    // Init core and setup.
    sym::point N;
    sym::init_get_order(N);

    // Perform tests.
    if (test_zp_from_int(N) != 1) return 1;
    if (test_transpose(N) != 1) return 1;
    if (test_identity(N) != 1) return 1;
    if (test_merge(N) != 1) return 1;
    if (test_multiply_vector(N) != 1) return 1;
    if (test_inverse(N) != 1) return 1;

    return 0;
}