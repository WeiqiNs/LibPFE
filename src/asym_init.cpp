#include "asym_init.hpp"

void asym::init_get_order(point N) {
    // Initiate relic.
    core_init();
    pc_param_set_any();

    // Get order.
    pc_get_ord(N);
}