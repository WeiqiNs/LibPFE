#pragma once

#include <gmp.h>

extern "C" {
#include "relic_sym/relic.h"
}

namespace sym {
    using point = bn_t;

    void init_get_order(point n);
}