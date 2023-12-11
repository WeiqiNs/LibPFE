#include "asym_ipfe_kim.hpp"

asym::ipfe::kim::Pp asym::ipfe::kim::ppgen(bool pre, int size, int bound) {
    asym::ipfe::kim::Pp pp{};
    pp.pre = pre;
    pp.size = size;
    pp.bound = bound;
    asym::init_get_order(pp.mod);
    asym::g1_gen(pp.g1_base);
    asym::g2_gen(pp.g2_base);
    if (pre) {
        pp.g1_table = asym::get_g1_pre_table(pp.g1_base);
        pp.g2_table = asym::get_g2_pre_table(pp.g2_base);
    }
    return pp;
}

asym::ipfe::kim::Sk asym::ipfe::kim::setup(asym::ipfe::kim::Pp pp) {
    asym::ipfe::kim::Sk sk{};
    sk.B = asym::matrix_zp_rand(pp.size, pp.size, pp.mod);
    sk.Bi = asym::matrix_transpose(asym::matrix_inverse_with_det(sk.B, sk.det, pp.size, pp.mod), pp.size, pp.size);
    sk.Bi = asym::matrix_multiply_constant(sk.Bi, sk.det, pp.size, pp.size);
    return sk;
}

asym::ipfe::kim::Key asym::ipfe::kim::keyGen(asym::ipfe::kim::Pp pp, asym::ipfe::kim::Sk sk, const int *function) {
    // Declare the returned key and convert message to Zp.
    asym::ipfe::kim::Key key{};
    asym::zpVec y = asym::vector_zp_from_int(function, pp.size, pp.mod);

    // Get a random point a and compute g1^(det * a).
    asym::Zp a = asym::zp_rand(pp.mod);
    asym::g1_mul(key.ctl, pp.g1_base, asym::zp_mul(a, sk.det));

    // Compute g1^(a * y * B).
    asym::zpVec yB = asym::matrix_multiply(y, sk.B, 1, pp.size, pp.size, pp.mod);
    asym::zpVec ayB = asym::matrix_multiply_constant(yB, a, 1, pp.size);
    if (pp.pre) key.ct = asym::vector_raise_g1_with_table(pp.g1_table, ayB, pp.size);
    else key.ct = asym::vector_raise_g1(pp.g1_base, ayB, pp.size);

    return key;
}

asym::ipfe::kim::Ct asym::ipfe::kim::enc(asym::ipfe::kim::Pp pp, asym::ipfe::kim::Sk sk, const int *message) {
    // Declare the returned ciphertext and convert message to Zp.
    asym::ipfe::kim::Ct ct{};
    asym::zpVec x = vector_zp_from_int(message, pp.size, pp.mod);

    // Get a random point b and compute g2^b.
    asym::Zp b = asym::zp_rand(pp.mod);
    asym::g2_mul(ct.ctr, pp.g2_base, b);

    // Compute g2^(bxBi).
    asym::zpVec xBi = asym::matrix_multiply(x, sk.Bi, 1, pp.size, pp.size, pp.mod);
    asym::zpVec bxBi = asym::matrix_multiply_constant(xBi, b, 1, pp.size);
    if (pp.pre) ct.ct = asym::vector_raise_g2_with_table(pp.g2_table, bxBi, pp.size);
    else ct.ct = asym::vector_raise_g2(pp.g2_base, bxBi, pp.size);

    return ct;
}

int asym::ipfe::kim::dec(asym::ipfe::kim::Pp pp, asym::ipfe::kim::Key y, asym::ipfe::kim::Ct x) {
    // Decrypt components.
    asym::gt xy, base;
    asym::inner_product(xy, y.ct, x.ct, pp.size);
    asym::bp_map(base, y.ctl, x.ctr);

    // Get a target group element holder.
    asym::gt output;

    // Iterate through a loop to find correct answer.
    for (int i = 1; i <= pp.bound; i++) {
        asym::gt_raise_int(output, base, i);
        if (asym::gt_compare(output, xy)) return i;
    }

    // Otherwise return 0 as the output.
    return 0;
}

