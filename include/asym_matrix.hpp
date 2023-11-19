#pragma once

#include "asym_field.hpp"

namespace asym {
    using zpMat = Zp *;

    /**
     * Convert a matrix of integers to a matrix of Zp elements.
     * @param int_mat - a matrix of integers.
     * @param row - the number of rows of the desired matrix.
     * @param col - the number of columns of the desired matrix.
     * @param mod - the size of the field.
     * @return a matrix of Zp elements.
     */
    zpMat matrix_zp_from_int(const int *int_mat, int row, int col, point mod);

    /**
     * Generate a random matrix with Zp elements.
     * @param row - the number of rows of the desired matrix.
     * @param col - the number of columns of the desired matrix.
     * @param mod - the size of the field.
     * @return a matrix of Zp elements.
     */
    zpMat matrix_zp_rand(int row, int col, point mod);

    /**
     * Generate an identity matrix.
     * @param size - the side length of the desired matrix.
     * @param mod - the size of the field.
     * @return an identity matrix of Zp elements.
     */
    zpMat matrix_identity(int size, point mod);

    /**
     * Check if a matrix is an identity matrix.
     * @param x - a matrix of Zp elements.
     * @param size - the side length of the input matrix.
     * @return 1 if the input matrix is an identity matrix and 0 otherwise.
     */
    int matrix_is_identity(zpMat x, int size);

    /**
     * Find the transpose of a matrix.
     * @param x - a matrix of Zp elements.
     * @param row - the number of rows of the input matrix x.
     * @param col - the number of columns of the input matrix x.
     * @return the matrix of x transposed.
     */
    zpMat matrix_transpose(zpMat x, int row, int col);

    /**
     * Join two matrices with the same number of rows.
     * @param x - a matrix of Zp elements.
     * @param y - a matrix of Zp elements.
     * @param row - the number of rows of both input matrices x and y.
     * @param col_x - the number of columns of the input matrix x.
     * @param col_y - the number of columns of the input matrix y.
     * @return the joined matrix of x and y.
     */
    zpMat matrix_join(zpMat x, zpMat y, int row, int col_x, int col_y);

    /**
     * Multiply two matrices.
     * @param x - a matrix of Zp elements.
     * @param y - a matrix of Zp elements.
     * @param row_x - the number of rows of the input matrix x.
     * @param row_y - the number of rows of the input matrix y, which should be the same as the number of columns of x.
     * @param col_y - the number of columns of the input matrix y.
     * @param mod - the size of the field.
     * @return the product of xy.
     */
    zpMat matrix_multiply(zpMat x, zpMat y, int row_x, int row_y, int col_y, point mod);

    /**
     * Multiply a matrix with a constant.
     * @param x - a matrix of Zp element.
     * @param c - a Zp element.
     * @param row - the number of rows of the input matrix x.
     * @param col - the number of columns of the input matrix x.
     * @param mod - the size of the field.
     * @return the multiplied matrix.
     */
    zpMat matrix_multiply_constant(zpMat x, Zp c, int row, int col);

    /**
     * Compute inverse of a square matrix using Gaussian elimination.
     * @param x - a matrix of Zp elements.
     * @param size - the side length of the input matrix.
     * @param mod - the size of the field.
     * @return the inverse of input matrix x.
     */
    zpMat matrix_inverse(zpMat x, int size, point mod);

    /**
     * Compute inverse of square matrix using Gaussian elimination and find the determinant in the process.
     * @param x - a matrix of Zp elements.
     * @param det - a placeholder for determinant of the matrix.
     * @param size - the side length of the input matrix.
     * @param mod - the size of the field.
     * @return the inverse of input matrix x.
     */
    zpMat matrix_inverse_with_det(zpMat x, Zp &det, int size, point mod);
}