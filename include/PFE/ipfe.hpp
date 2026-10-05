#ifndef PFE_IPFE_HPP
#define PFE_IPFE_HPP

#include <cstdint>
#include <vector>
#include <rbp/rbp.hpp>

namespace IPFE{
    using IntVec = std::vector<std::int64_t>;

    template <class C>
    [[nodiscard]] rbp::Vector<C> to_vector(const IntVec& x){
        return {x.begin(), x.end()};
    }
}

#endif
