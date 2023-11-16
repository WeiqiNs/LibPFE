#pragma once

#include "asym_init.hpp"

namespace asym {
    struct Zp {
        point num{};
        point mod{};
    };

    Zp zp_rand(point mod);

    Zp zp_zero(point mod);

    Zp zp_one(point mod);

    Zp zp_copy(Zp x);

    Zp zp_from_int(int x, point mod);

    Zp zp_add(Zp x, Zp y);

    Zp zp_neg(Zp x);

    Zp zp_mul(Zp x, Zp y);

    Zp zp_inv(Zp x);

    int zp_cmp(Zp x, Zp y);

    int zp_cmp_int(Zp x, int y);
}
