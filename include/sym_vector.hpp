#pragma once

#include "sym_field.hpp"
#include "sym_group.hpp"

namespace sym {
    using zpVec = Zp *;
    using gVec = g *;

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
     * Raise an element in G to a vector and obtain a vector in G.
     * @param base - an element in G.
     * @param x - a vector of Zp elements.
     * @param size - the size of input vector x.
     * @return a vector of G elements.
     */
    gVec vector_raise(g base, zpVec x, int size);

    /**
     * Compute e(a, b) where a, b are vectors with elements in G, and e is the bilinear map.
     * @param r - an element in Gt.
     * @param a - a vector of G elements.
     * @param b - a vector of G elements.
     * @param size - the size of the input vectors, which should be the same.
     */
    void inner_product(gt r, gVec a, gVec b, int size);
}