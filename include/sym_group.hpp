#pragma once

#include "sym_field.hpp"

namespace sym {
    using g = g1_t;
    using gt = gt_t;

    /**
     * Get a generator for the G group.
     * @param x - a holder for g element.
     */
    void g_gen(g x);

    /**
     * Multiply an element in G with a Zp element.
     * @param r - the resulting element in G.
     * @param x - an input element in G.
     * @param y - the Zp element to multiply with.
     */
    void g_mul(g r, g x, Zp y);

    /**
     * Get the unity element in Gt.
     * @param x - an input element in Gt.
     */
    void gt_get_unity(gt x);

    /**
     * Raise an element in Gt with a Zp element.
     * @param r - the resulting element in Gt.
     * @param x - an input element in Gt.
     * @param y - the Zp element to raise to.
     */
    void gt_raise(gt r, gt x, Zp y);

    /**
     * Raise an element in Gt with an integer.
     * @param r - the resulting element in Gt.
     * @param x - an input element in Gt.
     * @param y - the integer to raise to.
     */
    void gt_raise_int(gt r, gt x, int y);

    /**
     * Find inverse of a Gt element.
     * @param r - the resulting element in Gt.
     * @param x - an input element in Gt.
     */
    void gt_inverse(gt r, gt x);

    /**
     * Multiply two elements in Gt.
     * @param r - the resulting element in Gt.
     * @param x - an input element in Gt.
     * @param y - an input element in Gt.
     */
    void gt_multiply(gt r, gt x, gt y);

    /**
     * Compare two elements in Gt.
     * @param x - an input element in Gt.
     * @param y - an input element in Gt.
     * @return 1 if the two elements are the same and 0 otherwise.
     */
    int gt_compare(gt x, gt y);

    /**
     * Compute the bilinear pairing of element g1 and g2.
     * @param r - the resulting element in Gt.
     * @param x - an input element in G.
     * @param y - an input element in G.
     */
    void bp_map(gt r, g a, g b);
}