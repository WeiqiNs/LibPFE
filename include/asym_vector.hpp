#pragma once

#include "asym_field.hpp"
#include "asym_group.hpp"

namespace asym {
    using zpVec = asym::Zp *;

    using g1Vec = asym::g1 *;

    using g2Vec = asym::g2 *;

    zpVec vector_zp_from_int(const int *int_vec, int size, point mod);

    zpVec vector_zp_rand(int size, point mod);

    zpVec vector_merge(zpVec a, zpVec b, int size_a, int size_b);

    zpVec vector_add(zpVec a, zpVec b, int size);

    g1Vec vector_raise_g1(g1 base, zpVec x, int size);

    g2Vec vector_raise_g2(g2 base, zpVec x, int size);

    void inner_product(gt r, g1Vec a, g2Vec b, int size);
}