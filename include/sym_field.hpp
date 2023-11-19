#pragma once

#include "sym_init.hpp"

namespace sym {
    struct Zp {
        point num;
        point mod;
    };

    /**
     * Generate a random value in field Z_mod.
     * @param mod - the size of the field.
     * @return a random point in Z_mod.
     */
    Zp zp_rand(point mod);

    /**
     * Generate zero in field Z_mod.
     * @param mod - the size of the field.
     * @return zero in Z_mod.
     */
    Zp zp_zero(point mod);

    /**
     * Generate one in field Z_mod.
     * @param mod - the size of the field.
     * @return one in Z_mod.
     */
    Zp zp_one(point mod);

    /**
     * Return a copy of x.
     * @param x - a Zp element.
     * @return a copy of x.
     */
    Zp zp_copy(Zp x);

    /**
     * Generate a point in Z_mod equals to x.
     * @param x - a Zp element.
     * @param mod - size of the field.
     * @return a point in Z_mod equals to x.
     */
    Zp zp_from_int(int x, point mod);

    /**
     * Add two elements in Z_mod.
     * @param x - a Zp element.
     * @param y - a Zp element.
     * @return the added result element.
     */
    Zp zp_add(Zp x, Zp y);

    /**
     * Negate an element in Z_mod.
     * @param x - a Zp element.
     * @return the negated result element.
     */
    Zp zp_neg(Zp x);

    /**
     * Multiply two elements in Z_mod.
     * @param x - a Zp element.
     * @param y - a Zp element.
     * @return the result product element.
     */
    Zp zp_mul(Zp x, Zp y);

    /**
     * Find inverse of an element in Z_mod.
     * @param x - a Zp element.
     * @return the inverse element.
     */
    Zp zp_inv(Zp x);

    /**
     * Compare two elements from Zp.
     * @param x - a Zp element.
     * @param y - a Zp element.
     * @return 1 if these two values equal and 0 otherwise.
     */
    int zp_cmp(Zp x, Zp y);

    /**
     * Compare an element from Zp to an integer.
     * @param x - a Zp element.
     * @param y - a integer.
     * @return 1 if these two values equal and 0 otherwise.
     */
    int zp_cmp_int(Zp x, int y);
}