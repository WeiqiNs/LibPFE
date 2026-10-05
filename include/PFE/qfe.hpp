#ifndef PFE_QFE_HPP
#define PFE_QFE_HPP

#include <cstddef>
#include <vector>
#include "ipfe.hpp"

namespace QFE{
    using IPFE::IntVec;
    using IPFE::to_vector;
    using IntMat = std::vector<IntVec>;

    template <class C>
    [[nodiscard]] rbp::Matrix<C> to_matrix(const IntMat& f){
        std::vector<rbp::Vector<C>> rows;
        rows.reserve(f.size());
        for (const auto& row : f) rows.push_back(to_vector<C>(row));
        return rbp::Matrix<C>::from_rows(rows);
    }

    namespace detail{
        template <class C, rbp::Side S>
        [[nodiscard]] std::vector<rbp::Point<C, S>> masked(
            const std::vector<rbp::Point<C, S>>& base, const rbp::Zp<C>& r, const rbp::Vector<C>& m
        ){
            if (base.size() != m.size()) throw rbp::ShapeError("a masked vector needs one entry per base point");
            auto points = rbp::Point<C, S>::mul_generator(m);
            for (std::size_t i = 0; i < points.size(); ++i) points[i] += base[i] * r;
            return points;
        }

        template <class C>
        [[nodiscard]] rbp::Gt<C> bilinear(
            const std::vector<rbp::G1<C>>& p, const rbp::Matrix<C>& f, const std::vector<rbp::G2<C>>& q
        ){
            std::vector<rbp::G1<C>> combined;
            combined.reserve(f.cols());
            for (std::size_t j = 0; j < f.cols(); ++j){
                rbp::Vector<C> column(f.rows());
                for (std::size_t i = 0; i < f.rows(); ++i) column[i] = f.at(i, j);
                combined.push_back(rbp::msm(p, column));
            }
            return rbp::pair(combined, q);
        }
    }
}

#endif
