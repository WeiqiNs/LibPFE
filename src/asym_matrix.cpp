#include "asym_matrix.hpp"

asym::zpMat asym::matrix_zp_from_int(const int *int_mat, int row, int col, asym::point mod) {
    auto x = (asym::zpMat) malloc(sizeof(asym::Zp) * row * col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            x[i * col + j] = asym::zp_from_int(int_mat[i * col + j], mod);
        }
    }
    return x;
}

asym::zpMat asym::matrix_zp_rand(int row, int col, asym::point mod) {
    auto x = (asym::zpMat) malloc(sizeof(asym::Zp) * row * col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            x[i * col + j] = asym::zp_rand(mod);
        }
    }
    return x;
}

asym::zpMat asym::matrix_identity(int size, asym::point mod) {
    auto x = (asym::zpMat) malloc(sizeof(asym::Zp) * size * size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) x[i * size + j] = asym::zp_one(mod);
            else x[i * size + j] = asym::zp_zero(mod);
        }
    }
    return x;
}

int asym::matrix_is_identity(asym::zpMat x, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j && !asym::zp_cmp_int(x[i * size + j], 1)) return 0;
            if (i != j && !asym::zp_cmp_int(x[i * size + j], 0)) return 0;
        }
    }
    return 1;
}

asym::zpMat asym::matrix_transpose(asym::zpMat x, int row, int col) {
    auto xt = (asym::zpMat) malloc(sizeof(asym::Zp) * row * col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            xt[j * row + i] = asym::zp_copy(x[i * col + j]);
        }
    }
    return xt;
}

asym::zpMat asym::matrix_join(asym::zpMat x, asym::zpMat y, int row, int col_x, int col_y) {
    auto xy = (asym::zpMat) malloc(sizeof(asym::Zp) * row * (col_x + col_y));
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col_x; j++) {
            xy[i * (col_x + col_y) + j] = asym::zp_copy(x[i * col_x + j]);
        }
        for (int j = 0; j < col_y; j++) {
            xy[i * (col_x + col_y) + col_x + j] = asym::zp_copy(y[i * col_y + j]);
        }
    }
    return xy;
}

asym::zpMat asym::matrix_multiply(asym::zpMat x, asym::zpMat y, int row_x, int row_y, int col_y, asym::point mod) {
    auto xy = (asym::zpMat) malloc(sizeof(asym::Zp) * row_x * col_y);
    for (int i = 0; i < row_x; i++) {
        for (int j = 0; j < col_y; j++) {
            xy[i * row_y + j] = asym::zp_zero(mod);
            for (int k = 0; k < row_y; k++) {
                xy[i * col_y + j] = asym::zp_add(xy[i * col_y + j], asym::zp_mul(x[i * row_y + k], y[k * col_y + j]));
            }
        }
    }
    return xy;
}

asym::zpMat asym::matrix_multiply_constant(asym::zpMat x, asym::Zp c, int row, int col) {
    auto xc = (asym::zpMat) malloc(sizeof(asym::Zp) * row * col);
    for (int i = 0; i < row * col; i++) xc[i] = asym::zp_mul(x[i], c);
    return xc;
}


asym::zpMat asym::matrix_inverse(asym::zpMat x, int size, asym::point mod) {
    // Declare the row echelon matrix and generate it.
    asym::zpMat identity = matrix_identity(size, mod);
    asym::zpMat row_echelon = matrix_join(x, identity, size, size, size);

    // Declare temp value.
    asym::Zp temp_multiplier{};
    asym::Zp temp_neg{};

    // Bottom left half to all zeros.
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j && !asym::zp_cmp_int(row_echelon[i * 2 * size + j], 1)) {
                temp_multiplier = asym::zp_inv(row_echelon[i * 2 * size + i]);
                for (int k = i; k < size * 2; k++) {
                    row_echelon[j * 2 * size + k] = asym::zp_mul(row_echelon[j * 2 * size + k], temp_multiplier);
                }
            }

            if (i == j && asym::zp_cmp_int(row_echelon[i * 2 * size + j], 0)) break;

            if (i != j) {
                temp_multiplier = asym::zp_copy(row_echelon[j * 2 * size + i]);
                for (int k = i; k < size * 2; k++) {
                    temp_neg = asym::zp_mul(temp_multiplier, row_echelon[i * 2 * size + k]);
                    temp_neg = asym::zp_neg(temp_neg);
                    row_echelon[j * 2 * size + k] = asym::zp_add(row_echelon[j * 2 * size + k], temp_neg);
                }
            }
        }
    }

    // Top right half to all zeros.
    for (int i = size - 1; i > 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            temp_multiplier = asym::zp_copy(row_echelon[j * 2 * size + i]);
            for (int k = i; k < size * 2; k++) {
                temp_neg = asym::zp_mul(temp_multiplier, row_echelon[i * 2 * size + k]);
                temp_neg = asym::zp_neg(temp_neg);
                row_echelon[j * 2 * size + k] = asym::zp_add(row_echelon[j * 2 * size + k], temp_neg);
            }
        }
    }

    // Copy over the output.
    auto xi = (asym::zpMat) malloc(sizeof(asym::Zp) * size * size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            xi[i * size + j] = asym::zp_copy(row_echelon[i * 2 * size + size + j]);
        }
    }
    return xi;
}

asym::zpMat asym::matrix_inverse_with_det(asym::zpMat x, asym::Zp &det, int size, asym::point mod) {
    // Declare the row echelon matrix and generate it.
    asym::zpMat identity = matrix_identity(size, mod);
    asym::zpMat row_echelon = matrix_join(x, identity, size, size, size);

    // Declare temp value.
    asym::Zp temp_multiplier{};
    asym::Zp temp_neg{};

    // Set determinant to 1.
    det = asym::zp_copy(asym::zp_from_int(1, mod));

    // Bottom left half to all zeros.
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j && !asym::zp_cmp_int(row_echelon[i * 2 * size + j], 1)) {
                // Compute determinant.
                det = asym::zp_mul(det, row_echelon[i * 2 * size + j]);
                temp_multiplier = asym::zp_inv(row_echelon[i * 2 * size + i]);
                for (int k = i; k < size * 2; k++) {
                    row_echelon[j * 2 * size + k] = asym::zp_mul(row_echelon[j * 2 * size + k], temp_multiplier);
                }
            }

            if (i == j && asym::zp_cmp_int(row_echelon[i * 2 * size + j], 0)) break;

            if (i != j) {
                temp_multiplier = asym::zp_copy(row_echelon[j * 2 * size + i]);
                for (int k = i; k < size * 2; k++) {
                    temp_neg = asym::zp_mul(temp_multiplier, row_echelon[i * 2 * size + k]);
                    temp_neg = asym::zp_neg(temp_neg);
                    row_echelon[j * 2 * size + k] = asym::zp_add(row_echelon[j * 2 * size + k], temp_neg);
                }
            }
        }
    }

    // Top right half to all zeros.
    for (int i = size - 1; i > 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            temp_multiplier = asym::zp_copy(row_echelon[j * 2 * size + i]);
            for (int k = i; k < size * 2; k++) {
                temp_neg = asym::zp_mul(temp_multiplier, row_echelon[i * 2 * size + k]);
                temp_neg = asym::zp_neg(temp_neg);
                row_echelon[j * 2 * size + k] = asym::zp_add(row_echelon[j * 2 * size + k], temp_neg);
            }
        }
    }

    // Copy over the output.
    auto xi = (asym::zpMat) malloc(sizeof(asym::Zp) * size * size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            xi[i * size + j] = asym::zp_copy(row_echelon[i * 2 * size + size + j]);
        }
    }
    return xi;
}