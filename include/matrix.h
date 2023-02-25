#ifndef PPANN_MATRIX_H
#define PPANN_MATRIX_H

#include "field.h"
#include "group.h"

typedef zp* zp_mat;

void matrix_zp_from_int(zp_mat x, int *int_mat, int row, int col);

void matrix_zp_rand(zp_mat x, int row, int col);

void matrix_identity(zp_mat x, int size);

int matrix_is_identity(zp_mat x, int size);

void matrix_transpose(zp_mat xt, zp_mat x, int row, int col);

void matrix_merge(zp_mat xy, zp_mat x, zp_mat y, int row, int col_x, int col_y);

void matrix_multiply(zp_mat xy, zp_mat x, zp_mat y, int row_x, int row_y, int col_y);

void matrix_inverse(zp_mat xi, zp_mat x, int size);

#endif //PPANN_MATRIX_H
