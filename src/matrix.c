#include "matrix.h"

zp_mat matrix_zp_from_int(const int *int_mat, int row, int col, bn_st *modular) {
    zp_mat x;
    x = (zp_mat) malloc(sizeof(struct zp) * row * col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            x[i * col + j] = zp_from_int(int_mat[i * col + j], modular);
        }
    }
    return x;
}

zp_mat matrix_zp_rand(int row, int col, bn_st *modular) {
    zp_mat x;
    x = (zp_mat) malloc(sizeof(struct zp) * row * col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            x[i * col + j] = rand_zp(modular);
        }
    }
    return x;
}

zp_mat matrix_identity(int size, bn_st *modular) {
    zp_mat x;
    x = (zp_mat) malloc(sizeof(struct zp) * size * size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) x[i * size + j] = zp_one(modular);
            else x[i * size + j] = zp_zero(modular);
        }
    }
    return x;
}

int matrix_is_identity(zp_mat x, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j && !zp_cmp_int(x[i * size + j], 1)) return 0;
            if (i != j && !zp_cmp_int(x[i * size + j], 0)) return 0;
        }
    }
    return 1;
}

zp_mat matrix_transpose(zp_mat x, int row, int col) {
    zp_mat xt;
    xt = (zp_mat) malloc(sizeof(struct zp) * row * col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            xt[j * row + i] = zp_copy(x[i * col + j]);
        }
    }
    return xt;
}

zp_mat matrix_merge(zp_mat x, zp_mat y, int row, int col_x, int col_y) {
    zp_mat xy;
    xy = (zp_mat) malloc(sizeof(struct zp) * row * (col_x + col_y));
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col_x; j++) {
            xy[i * (col_x + col_y) + j] = zp_copy(x[i * col_x + j]);
        }
        for (int j = 0; j < col_y; j++) {
            xy[i * (col_x + col_y) + col_x + j] = zp_copy(y[i * col_y + j]);
        }
    }
    return xy;
}

zp_mat matrix_multiply(zp_mat x, zp_mat y, int row_x, int row_y, int col_y, bn_st *modular) {
    zp_mat xy = (zp_mat) malloc(sizeof(struct zp) * row_x * col_y);

    for (int i = 0; i < row_x; i++) {
        for (int j = 0; j < col_y; j++) {
            xy[i * row_y + j] = zp_zero(modular);
            for (int k = 0; k < row_y; k++) {
                xy[i * col_y + j] = zp_add(xy[i * col_y + j], zp_mul(x[i * row_y + k], y[k * col_y + j]));
            }
        }
    }
    return xy;
}

zp_mat matrix_inverse(zp_mat x, int size, bn_st *modular) {
    // Declare the row echelon matrix and generate it.
    zp_mat identity = matrix_identity(size, modular);
    zp_mat row_echelon = matrix_merge(x, identity, size, size, size);

    // Declare temp value.
    struct zp temp_multiplier, temp_neg;

    // Bottom left half to all zeros.
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j && !zp_cmp_int(row_echelon[i * 2 * size + j], 1)) {
                temp_multiplier = zp_inv(row_echelon[i * 2 * size + i]);
                for (int k = i; k < size * 2; k++) {
                    row_echelon[j * 2 * size + k] = zp_mul(row_echelon[j * 2 * size + k], temp_multiplier);
                }
            }

            if (i == j && zp_cmp_int(row_echelon[i * 2 * size + j], 0)) break;

            if (i != j) {
                temp_multiplier = zp_copy(row_echelon[j * 2 * size + i]);
                for (int k = i; k < size * 2; k++) {
                    temp_neg = zp_mul(temp_multiplier, row_echelon[i * 2 * size + k]);
                    temp_neg = zp_neg(temp_neg);
                    row_echelon[j * 2 * size + k] = zp_add(row_echelon[j * 2 * size + k], temp_neg);
                }
            }
        }
    }

    // Top right half to all zeros.
    for (int i = size - 1; i > 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            temp_multiplier = zp_copy(row_echelon[j * 2 * size + i]);
            for (int k = i; k < size * 2; k++) {
                temp_neg = zp_mul(temp_multiplier, row_echelon[i * 2 * size + k]);
                temp_neg = zp_neg(temp_neg);
                row_echelon[j * 2 * size + k] = zp_add(row_echelon[j * 2 * size + k], temp_neg);
            }
        }
    }

    // Copy over the output.
    zp_mat xi;
    xi = (zp_mat) malloc(sizeof(struct zp) * size * size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            xi[i * size + j] = zp_copy(row_echelon[i * 2 * size + size + j]);
        }
    }
    return xi;
}