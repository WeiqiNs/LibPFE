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

            void add(const std::vector<rbp::G1<C>>& ps, const std::vector<rbp::G2<C>>& qs){
                if (ps.size() != qs.size()) throw rbp::ShapeError("a pairing product needs one G2 point per G1 point");
                lefts.insert(lefts.end(), ps.begin(), ps.end());
                rights.insert(rights.end(), qs.begin(), qs.end());
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
    }
}

#endif
