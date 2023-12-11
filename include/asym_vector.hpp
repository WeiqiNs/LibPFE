#pragma once

#include "asym_field.hpp"
#include "asym_group.hpp"

namespace asym {
    using zpVec = Zp *;
    using g1Vec = g1 *;
    using g2Vec = g2 *;

    /**
     * Build a pre-computation table for faster multiplication in G1.
     * @param base - an element in G1.
     * @return a vector of pre computed elements.
     */
    g1Vec get_g1_pre_table(g1 base);

    /**
     * Build a pre-computation table for faster multiplication in G2.
     * @param base - an element in G2.
     * @return a vector of pre computed elements.
     */
    g2Vec get_g2_pre_table(g2 base);

    /**
     * Generate a vector of Zp elements from a vector of int.
     * @param int_vec - a vector of integers.
     * @param size - the length of the input vector.
     * @param mod - the size of the field.
     * @return a vector of Zp elements defined by input.
     */
    zpVec vector_zp_from_int(const int *int_vec, int size, point mod);

    /**
     * Generate a vector of random Zp elements.
     * @param size - the length of the input vector.
     * @param mod - the size of the field.
     * @return a vector of random Zp elements.
     */
    zpVec vector_zp_rand(int size, point mod);

    /**
     * Join two vectors.
     * @param a - a vector of Zp elements.
     * @param b - a vector of Zp elements.
     * @param size_a - the length of input vector a.
     * @param size_b - the length of input vector b.
     * @return the joined vector of a and b.
     */
    zpVec vector_join(zpVec a, zpVec b, int size_a, int size_b);

    /**
     * Add two vectors.
     * @param a - a vector of Zp elements.
     * @param b - a vector of Zp elements.
     * @param size - the size of the input vectors, which should be the same.
     * @return the added result of a and b.
     */
    zpVec vector_add(zpVec a, zpVec b, int size);

    /**
     * Raise an element in G1 to a vector and obtain a vector in G1.
     * @param base - an element in G1.
     * @param x - a vector of Zp elements.
     * @param size - the size of input vector x.
     * @return a vector of G1 elements.
     */
    g1Vec vector_raise_g1(g1 base, zpVec x, int size);

    /**
     * Raise an element in G1 to a vector and obtain a vector in G1.
     * @param table - a pre-computed table with values raised from desired base in G1.
     * @param x - a vector of Zp elements.
     * @param size - the size of input vector x.
     * @return a vector of G1 elements.
     */
    g1Vec vector_raise_g1_with_table(g1Vec table, zpVec x, int size);

    /**
     * Raise an element in G2 to a vector and obtain a vector in G2.
     * @param base - an element in G2.
     * @param x - a vector of Zp elements.
     * @param size - the size of input vector x.
     * @return a vector of G2 elements.
     */
    g2Vec vector_raise_g2(g2 base, zpVec x, int size);

    /**
     * Raise an element in G2 to a vector and obtain a vector in G2.
     * @param table - a pre-computed table with values raised from desired base in G2.
     * @param x - a vector of Zp elements.
     * @param size - the size of input vector x.
     * @return a vector of G2 elements.
     */
    g2Vec vector_raise_g2_with_table(g2Vec table, zpVec x, int size);

    /**
     * Compute e(a, b) where a, b are vectors with elements in G1 and G2 respectively, and e is the bilinear map.
     * @param r - an element in Gt.
     * @param a - a vector of G1 elements.
     * @param b - a vector of G2 elements.
     * @param size - the size of the input vectors, which should be the same.
     */
    void inner_product(gt r, g1Vec a, g2Vec b, int size);
}