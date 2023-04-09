#ifndef PPANN_MATRIX_H
#define PPANN_MATRIX_H

#include "field.h"
#include "group.h"

typedef zp *zp_mat;

zp_mat matrix_zp_from_int(const int *int_mat, int row, int col, bn_t modular);

zp_mat matrix_zp_rand(int row, int col, bn_t modular);

zp_mat matrix_identity(int size, bn_t modular);

int matrix_is_identity(zp_mat x, int size);

zp_mat matrix_transpose(zp_mat x, int row, int col);

zp_mat matrix_merge(zp_mat x, zp_mat y, int row, int col_x, int col_y);

zp_mat matrix_multiply(zp_mat x, zp_mat y, int row_x, int row_y, int col_y, bn_t modular);

zp_mat matrix_inverse(zp_mat x, int size, bn_t modular);

#endif //PPANN_MATRIX_H