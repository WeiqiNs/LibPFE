#include "field.h"
#include "group.h"
#include <stdio.h>
#include "relic/relic.h"


int main() {
    core_init();
    pc_param_set_any();

//
//    g a, b;
//    gt x, y;
//
//    // Get generator and map to target group, x =
//    generator(a);
//    map(a, a, x);
//
//
//    g1_print(a);
//    printf("\n");
//    gt_print(x);
//    printf("\n");
//
//    // Multiply and Exp.
//    zp m, n;
//    zp_from_int(m, 5);
//    zp_from_int(n, 5);
//
//    // b = a^5, y = gt^10
//    multiply(b, a, m);
//
//    g1_print(b);
//    printf("\n");
//
//    exponentiation(y, x, n);
//
//    gt_free(x);
//    map(b, b, x);
//
//    printf("Compare: %i", gt_cmp(y, x));

    g1_t a, b;
    gt_t x, y, z;

    g1_get_gen(a);

    g1_print(a);

    g1_mul_dig(b, a, 5);

    g1_print(b);

    pc_map(x, a, a);

    pc_map(y, b, b);

    gt_exp_dig(z, x, 25);

    printf("Compare: %i", gt_cmp(y, z) == RLC_EQ);

    return 0;
}