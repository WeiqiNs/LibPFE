#include "asym_matrix.hpp"

int test_zp_from_int(asym::point N) {
    int int_mat[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    asym::zpMat x = asym::matrix_zp_from_int(int_mat, 3, 3, N);
    return asym::zp_cmp_int(x[8], 9);
}

int test_transpose(asym::point N) {
    int row = 3, col = 3;
    asym::zpMat x = asym::matrix_zp_rand(row, col, N);
    asym::zpMat xt = asym::matrix_transpose(x, row, col);
    return asym::zp_cmp(xt[col - 1], x[2 * row]);
}

int test_identity(asym::point N) {
    int size = 10;
    asym::zpMat x = asym::matrix_identity(size, N);
    return asym::matrix_is_identity(x, size);
}

int test_merge(asym::point N) {
    int size = 10;
    asym::zpMat x = asym::matrix_zp_rand(size, size, N);
    asym::zpMat y = asym::matrix_identity(size, N);
    asym::zpMat xy = asym::matrix_join(x, y, size, size, size);
    return asym::zp_cmp(x[2 * size + 1], xy[4 * size + 1]);
}

int test_multiply_vector(asym::point N) {
    int mat_x[5] = {1, 2, 3, 4, 5};
    int mat_y[15] = {
            10, 20, 30,
            10, 20, 30,
            10, 20, 30,
            10, 20, 30,
            10, 20, 30
    };

    asym::zpMat x = asym::matrix_zp_from_int(mat_x, 1, 5, N);
    asym::zpMat y = asym::matrix_zp_from_int(mat_y, 5, 3, N);
    asym::zpMat xy = asym::matrix_multiply(x, y, 1, 5, 3, N);
    return asym::zp_cmp_int(xy[2], 450);
}

int test_multiply_constant(asym::point N) {
    asym::Zp c = asym::zp_from_int(11, N);

    int mat_x[6] = {1, 2, 3, 4, 5, 6};
    asym::zpMat x = asym::matrix_zp_from_int(mat_x, 1, 6, N);
    asym::zpMat xy = asym::matrix_multiply_constant(x, c, 2, 3);
    return asym::zp_cmp_int(xy[5], 66);
}

int test_inverse(asym::point N) {
    int size = 10;
    asym::zpMat x = asym::matrix_zp_rand(size, size, N);
    asym::zpMat xi = asym::matrix_inverse(x, size, N);
    asym::zpMat r = asym::matrix_multiply(x, xi, size, size, size, N);
    return asym::matrix_is_identity(r, size);
}

int test_inverse_with_det(asym::point N) {
    int size = 3;
    int mat[9] = {3, 5, 8, 2, 2, 2, 9, 9, 3};
    asym::zpMat x = asym::matrix_zp_from_int(mat, size, size, N);

    asym::Zp det{};
    asym::matrix_inverse_with_det(x, det, size, N);

    return asym::zp_cmp_int(det, 24) == 1;
}

int main() {
    // Init core and setup.
    asym::point N;
    asym::init_get_order(N);

    // Perform tests.
    if (test_zp_from_int(N) != 1) return 1;
    if (test_transpose(N) != 1) return 1;
    if (test_identity(N) != 1) return 1;
    if (test_merge(N) != 1) return 1;
    if (test_multiply_vector(N) != 1) return 1;
    if (test_multiply_constant(N) != 1) return 1;
    if (test_inverse(N) != 1) return 1;
    if (test_inverse_with_det(N) != 1) return 1;

    return 0;
}