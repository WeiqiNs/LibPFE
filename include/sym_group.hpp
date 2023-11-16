#pragma once

#include "sym_field.hpp"

namespace sym {
    using g = g1_t;

    using gt = gt_t;

    void g_gen(g x);

    void g_mul(g r, g x, Zp y);

    void gt_raise(gt r, gt x, Zp y);

    void bp_map(gt r, g a, g b);
}