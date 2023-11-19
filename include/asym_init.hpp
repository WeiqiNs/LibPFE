#pragma once

#include <gmp.h>

extern "C" {
#include "relic_asym/relic.h"
}

namespace asym {
    using point = bn_t;

    /**
     * Initialize the relic curve and get its order.
     * @param n: the order of the group simulated by selected curve.
     */
    void init_get_order(point n);
}