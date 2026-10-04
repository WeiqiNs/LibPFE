#include <iostream>
#include <RIPFE/ipfe_opt.hpp>

template <class C>
bool inner_product(){
    const auto msk = IPFE::OPT::setup<C>(10);
    const rbp::DlogTable<C> table(IPFE::OPT::base<C>(), 300, 400);
    const auto sk = IPFE::OPT::keygen(msk, IPFE::IntVec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    const auto ct = IPFE::OPT::enc(msk, IPFE::IntVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    const bool ok = IPFE::OPT::dec(table, sk, ct) == 330;
    std::cout << C::name << (ok ? ": IPFE computation succeeded" : ": IPFE computation failed") << std::endl;
    return ok;
}

template <class... Curves>
bool on_every_curve(){
    return (inner_product<Curves>() & ...);
}

int main(){
    return on_every_curve<rbp::BLS12_381, rbp::SS1536, rbp::BN254>() ? 0 : 1;
}
