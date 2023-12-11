#include "asym_ipfe_tao.hpp"

asym::ipfe::tao::Pp asym::ipfe::tao::ppgen(bool pre, int size, int bound) {
    asym::ipfe::tao::Pp pp{};
    pp.pre = pre;
    pp.size = size;
    pp.bound = bound;
    pp.c_size = size * 2 + 5;
    asym::init_get_order(pp.mod);
    asym::g1_gen(pp.g1_base);
    asym::g2_gen(pp.g2_base);
    asym::bp_map(pp.gt_base, pp.g1_base, pp.g2_base);
    if (pre) {
        pp.g1_table = asym::get_g1_pre_table(pp.g1_base);
        pp.g2_table = asym::get_g2_pre_table(pp.g2_base);
    }
    return pp;
}

asym::ipfe::tao::Sk asym::ipfe::tao::setup(asym::ipfe::tao::Pp pp) {
    asym::ipfe::tao::Sk sk{};
    sk.B = asym::matrix_zp_rand(pp.c_size, pp.c_size, pp.mod);
    sk.Bi = asym::matrix_transpose(asym::matrix_inverse(sk.B, pp.c_size, pp.mod), pp.c_size, pp.c_size);
    return sk;
}

asym::ipfe::tao::Key asym::ipfe::tao::keyGen(asym::ipfe::tao::Pp pp, asym::ipfe::tao::Sk sk, const int *function) {
    // Declare the returned key and convert message to Zp.
    asym::ipfe::tao::Key key{};
    asym::zpVec y = asym::vector_zp_from_int(function, pp.size, pp.mod);

    // Get desired values.
    int zero[] = {0};
    asym::zpVec zero_vec = asym::vector_zp_from_int(zero, 1, pp.mod);
    asym::zpVec rand_vec = asym::vector_zp_rand(2, pp.mod);

    // Get n + 2 zeros in Zp.
    int zero_list[pp.size + 2];
    for (int i = 0; i < pp.size + 2; ++i) { zero_list[i] = 0; }

    // Merge zeros with y to get new y.
    y = asym::vector_join(y, asym::vector_zp_from_int(zero_list, pp.size + 2, pp.mod), pp.size, pp.size + 2);

    // Merge the random values.
    y = asym::vector_join(y, rand_vec, pp.size * 2 + 2, 2);
    y = asym::vector_join(y, zero_vec, pp.size * 2 + 4, 1);

    // Compute g1^yB.
    asym::zpMat yB = asym::matrix_multiply(y, sk.B, 1, pp.c_size, pp.c_size, pp.mod);
    if (pp.pre) key.ct = asym::vector_raise_g1_with_table(pp.g1_table, yB, pp.c_size);
    else key.ct = asym::vector_raise_g1(pp.g1_base, yB, pp.c_size);

    return key;
}

asym::ipfe::tao::Ct asym::ipfe::tao::enc(asym::ipfe::tao::Pp pp, asym::ipfe::tao::Sk sk, const int *message) {
    // Declare the returned ciphertext and convert message to Zp.
    asym::ipfe::tao::Ct ct{};
    asym::zpVec x = asym::vector_zp_from_int(message, pp.size, pp.mod);

    // Get desired values.
    int zero[] = {0};
    asym::zpVec zero_vec = asym::vector_zp_from_int(zero, 1, pp.mod);
    asym::zpVec rand_vec = asym::vector_zp_rand(2, pp.mod);

    // Get n zeros in Zp.
    int zero_list[pp.size];
    for (int i = 0; i < pp.size; ++i) { zero_list[i] = 0; }

    // Merge zeros with x to get new x.
    x = asym::vector_join(x, asym::vector_zp_from_int(zero_list, pp.size, pp.mod), pp.size, pp.size);

    // Merge the random values and three zeros.
    x = asym::vector_join(x, rand_vec, pp.size * 2, 2);
    x = asym::vector_join(x, zero_vec, pp.size * 2 + 2, 1);
    x = asym::vector_join(x, zero_vec, pp.size * 2 + 3, 1);
    x = asym::vector_join(x, zero_vec, pp.size * 2 + 4, 1);

    // Compute g2^xBi.
    asym::zpMat xBi = asym::matrix_multiply(x, sk.Bi, 1, pp.c_size, pp.c_size, pp.mod);
    if (pp.pre) ct.ct = asym::vector_raise_g2_with_table(pp.g2_table, xBi, pp.c_size);
    else ct.ct = asym::vector_raise_g2(pp.g2_base, xBi, pp.c_size);

    return ct;
}

int asym::ipfe::tao::dec(asym::ipfe::tao::Pp pp, asym::ipfe::tao::Key y, asym::ipfe::tao::Ct x) {
    // Decrypt components.
    asym::gt xy;
    asym::inner_product(xy, y.ct, x.ct, pp.c_size);

    // Get a target group element holder.
    asym::gt output;

    // Iterate through a loop to find correct answer.
    for (int i = 1; i <= pp.bound; i++) {
        asym::gt_raise_int(output, pp.gt_base, i);
        if (asym::gt_compare(output, xy)) return i;
    }

    // Otherwise return 0 as the output.
    return 0;
}

