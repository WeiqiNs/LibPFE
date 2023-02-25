#include "matrix.h"

void matrix_zp_from_int(zp_mat x, int *int_mat, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            zp_from_int(x[i * col + j], int_mat[i * col + j]);
        }
    }
}

void matrix_zp_rand(zp_mat x, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            rand_zp(x[i * col + j]);
        }
    }
}

void matrix_identity(zp_mat x, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) zp_from_int(x[i * size + j], 1);
            else zp_zero(x[i * size + j]);
        }
    }
}

int matrix_is_identity(zp_mat x, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j && fp_cmp_dig(x[i * size + j], 1) != RLC_EQ) return 0;
            if (i != j && fp_cmp_dig(x[i * size + j], 0) != RLC_EQ) return 0;
        }
    }
    return 1;
}

void matrix_transpose(zp_mat xt, zp_mat x, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            zp_copy(xt[j * row + i], x[i * col + j]);
        }
    }
}

void matrix_merge(zp_mat xy, zp_mat x, zp_mat y, int row, int col_x, int col_y) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col_x; j++) {
            zp_copy(xy[i * (col_x + col_y) + j], x[i * col_x + j]);
        }
        for (int j = 0; j < col_y; j++) {
            zp_copy(xy[i * (col_x + col_y) + col_x + j], y[i * col_y + j]);
        }
    }
}

void matrix_multiply(zp_mat xy, zp_mat x, zp_mat y, int row_x, int row_y, int col_y) {
    zp temp;
    for (int i = 0; i < row_x; i++) {
        for (int j = 0; j < col_y; j++) {
            zp_from_int(xy[i * row_y + j], 0);
            for (int k = 0; k < row_y; k++) {
                zp_multiply(temp, x[i * row_y + k], y[k * col_y + j]);
                zp_add(xy[i * col_y + j], xy[i * col_y + j], temp);
            }
        }
    }
}

void matrix_inverse(zp_mat xi, zp_mat x, int size) {
    // Declare the row echelon matrix and generate it.
    zp *identity, *row_echelon;
    identity = (zp *) malloc(size * size * sizeof(zp));
    row_echelon = (zp *) malloc(2 * size * size * sizeof(zp));
    matrix_identity(identity, size);
    matrix_merge(row_echelon, x, identity, size, size, size);

    // Declare temp value.
    zp temp_multiplier, temp_neg;

    // Bottom left half to all zeros.
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j && fp_cmp_dig(row_echelon[i * 2 * size + j], 1) != RLC_EQ) {
                zp_inverse(temp_multiplier, row_echelon[i * 2 * size + i]);
                for (int k = i; k < size * 2; k++) {
                    zp_multiply(row_echelon[j * 2 * size + k], row_echelon[j * 2 * size + k], temp_multiplier);
                }
            }

            if (i == j && fp_cmp_dig(row_echelon[i * 2 * size + j], 0) == RLC_EQ) break;

            if (i != j) {
                zp_copy(temp_multiplier, row_echelon[j * 2 * size + i]);
                for (int k = i; k < size * 2; k++) {
                    zp_multiply(temp_neg, temp_multiplier, row_echelon[i * 2 * size + k]);
                    fp_neg(temp_neg, temp_neg);
                    zp_add(row_echelon[j * 2 * size + k], row_echelon[j * 2 * size + k], temp_neg);
                }
            }
        }
    }

    // Top right half to all zeros.
    for (int i = size - 1; i > 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            zp_copy(temp_multiplier, row_echelon[j * 2 * size + i]);
            for (int k = i; k < size * 2; k++) {
                zp_multiply(temp_neg, temp_multiplier, row_echelon[i * 2 * size + k]);
                fp_neg(temp_neg, temp_neg);
                zp_add(row_echelon[j * 2 * size + k], row_echelon[j * 2 * size + k], temp_neg);
            }
        }
    }

    // Copy over the output.
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            zp_copy(xi[i * size + j], row_echelon[i * 2 * size + size + j]);
        }
    }
}
