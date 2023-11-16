#pragma once

#include "sym_field.hpp"
#include "sym_group.hpp"

namespace sym {
    using zpVec = sym::Zp *;

    using gVec = sym::g *;

    zpVec vector_zp_from_int(const int *int_vec, int size, point mod);

    zpVec vector_zp_rand(int size, point mod);

    zpVec vector_merge(zpVec a, zpVec b, int size_a, int size_b);

    zpVec vector_add(zpVec a, zpVec b, int size);

    gVec vector_raise(g base, zpVec x, int size);

    void inner_product(gt r, gVec a, gVec b, int size);
}