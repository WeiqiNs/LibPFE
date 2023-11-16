#pragma once

#include "asym_field.hpp"

namespace asym {
    using g1 = g1_t;

    using g2 = g2_t;

    using gt = gt_t;

    void g1_gen(g1 x);

    void g2_gen(g2 x);

    void g1_mul(g1 r, g1 x, Zp y);

    void g2_mul(g2 r, g2 x, Zp y);

    void gt_raise(gt r, gt x, Zp y);

    void bp_map(gt r, g1 x, g2 y);
}