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

    namespace detail{
        template <class C>
        struct PairingProduct{
            std::vector<rbp::G1<C>> lefts;
            std::vector<rbp::G2<C>> rights;

            void add(const rbp::G1<C>& p, const rbp::G2<C>& q){
                lefts.push_back(p);
                rights.push_back(q);
            }

            [[nodiscard]] rbp::Gt<C> evaluate() const{
                return rbp::pair(lefts, rights);
            }
        };

        template <class C, rbp::Side S>
        [[nodiscard]] std::vector<rbp::Point<C, S>> negated(std::vector<rbp::Point<C, S>> points){
            for (auto& point : points) point = -point;
            return points;
        }

        template <class C, rbp::Side S>
        [[nodiscard]] std::vector<rbp::Point<C, S>> joined(
            std::vector<rbp::Point<C, S>> first, const std::vector<rbp::Point<C, S>>& second
        ){
            first.insert(first.end(), second.begin(), second.end());
            return first;
        }
    }
}

#endif
