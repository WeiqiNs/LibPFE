#pragma once

#include <gmp.h>

extern "C" {
#include "relic_asym/relic.h"
}

namespace asym {
    using point = bn_t;

    void init_get_order(point n);
}