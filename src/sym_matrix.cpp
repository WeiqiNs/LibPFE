#include "sym_matrix.hpp"

sym::zpMat sym::matrix_zp_from_int(const int *int_mat, int row, int col, sym::point mod) {
    auto x = (sym::zpMat) malloc(sizeof(sym::Zp) * row * col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            x[i * col + j] = sym::zp_from_int(int_mat[i * col + j], mod);
        }
    }
    return x;
}

sym::zpMat sym::matrix_zp_rand(int row, int col, sym::point mod) {
    auto x = (sym::zpMat) malloc(sizeof(sym::Zp) * row * col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            x[i * col + j] = sym::zp_rand(mod);
        }
    }
    return x;
}

sym::zpMat sym::matrix_identity(int size, sym::point mod) {
    auto x = (sym::zpMat) malloc(sizeof(sym::Zp) * size * size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) x[i * size + j] = sym::zp_one(mod);
            else x[i * size + j] = sym::zp_zero(mod);
        }
    }
    return x;
}

int sym::matrix_is_identity(sym::zpMat x, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j && !sym::zp_cmp_int(x[i * size + j], 1)) return 0;
            if (i != j && !sym::zp_cmp_int(x[i * size + j], 0)) return 0;
        }
    }
    return 1;
}

sym::zpMat sym::matrix_transpose(sym::zpMat x, int row, int col) {
    auto xt = (sym::zpMat) malloc(sizeof(sym::Zp) * row * col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            xt[j * row + i] = sym::zp_copy(x[i * col + j]);
        }
    }
    return xt;
}

sym::zpMat sym::matrix_join(sym::zpMat x, sym::zpMat y, int row, int col_x, int col_y) {
    auto xy = (sym::zpMat) malloc(sizeof(sym::Zp) * row * (col_x + col_y));
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col_x; j++) {
            xy[i * (col_x + col_y) + j] = sym::zp_copy(x[i * col_x + j]);
        }
        for (int j = 0; j < col_y; j++) {
            xy[i * (col_x + col_y) + col_x + j] = sym::zp_copy(y[i * col_y + j]);
        }
    }
    return xy;
}

sym::zpMat sym::matrix_multiply(sym::zpMat x, sym::zpMat y, int row_x, int row_y, int col_y, sym::point mod) {
    auto xy = (sym::zpMat) malloc(sizeof(sym::Zp) * row_x * col_y);
    for (int i = 0; i < row_x; i++) {
        for (int j = 0; j < col_y; j++) {
            xy[i * row_y + j] = sym::zp_zero(mod);
            for (int k = 0; k < row_y; k++) {
                xy[i * col_y + j] = sym::zp_add(xy[i * col_y + j], zp_mul(x[i * row_y + k], y[k * col_y + j]));
            }
        }
    }
    return xy;
}

sym::zpMat sym::matrix_inverse(sym::zpMat x, int size, sym::point mod) {
    // Declare the row echelon matrix and generate it.
    sym::zpMat identity = sym::matrix_identity(size, mod);
    sym::zpMat row_echelon = sym::matrix_join(x, identity, size, size, size);

    // Declare temp value.
    sym::Zp temp_multiplier{};
    sym::Zp temp_neg{};

    // Bottom left half to all zeros.
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j && !sym::zp_cmp_int(row_echelon[i * 2 * size + j], 1)) {
                temp_multiplier = sym::zp_inv(row_echelon[i * 2 * size + i]);
                for (int k = i; k < size * 2; k++) {
                    row_echelon[j * 2 * size + k] = sym::zp_mul(row_echelon[j * 2 * size + k], temp_multiplier);
                }
            }

            if (i == j && sym::zp_cmp_int(row_echelon[i * 2 * size + j], 0)) break;

            if (i != j) {
                temp_multiplier = sym::zp_copy(row_echelon[j * 2 * size + i]);
                for (int k = i; k < size * 2; k++) {
                    temp_neg = sym::zp_mul(temp_multiplier, row_echelon[i * 2 * size + k]);
                    temp_neg = sym::zp_neg(temp_neg);
                    row_echelon[j * 2 * size + k] = sym::zp_add(row_echelon[j * 2 * size + k], temp_neg);
                }
            }
        }
    }

    // Top right half to all zeros.
    for (int i = size - 1; i > 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            temp_multiplier = sym::zp_copy(row_echelon[j * 2 * size + i]);
            for (int k = i; k < size * 2; k++) {
                temp_neg = sym::zp_mul(temp_multiplier, row_echelon[i * 2 * size + k]);
                temp_neg = sym::zp_neg(temp_neg);
                row_echelon[j * 2 * size + k] = sym::zp_add(row_echelon[j * 2 * size + k], temp_neg);
            }
        }
    }

    // Copy over the output.
    auto xi = (sym::zpMat) malloc(sizeof(sym::Zp) * size * size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            xi[i * size + j] = sym::zp_copy(row_echelon[i * 2 * size + size + j]);
        }
    }
    return xi;
}