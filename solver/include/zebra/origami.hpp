
#ifndef SOLVER_ORIGAMI_HPP
#define SOLVER_ORIGAMI_HPP

#include <zebra/geometry.hpp>
#include <zebra/solution.hpp>

#include <CGAL/Boolean_set_operations_2.h>

namespace zebra
{
class origami
{
public:
    solution sol = make_solution_1();

    std::vector<origami> move_to(point p) const
    {
        std::vector<origami> results;
        results.reserve(sol.destination_positions.size() * 2);
        for (auto dp : sol.destination_positions) {
        {
            transformation move(CGAL::TRANSLATION, p - dp);
            auto moved = *this;
            moved.sol.transform(move);
            results.push_back(moved);
        }
        return results;
    }

    double resemblance(polygon target) const
    {
        polygon_with_holes p_or;
        std::vector<polygon_with_holes> p_ands;
        CGAL::join(poly, target, p_or);
        CGAL::intersection(poly, target, std::back_inserter(p_ands));
        CGAL::Gmpq and_area = 0;
        for (const auto& holy : p_ands)
        {
            and_area += holy.outer_boundary().area();
        }
        auto or_area = p_or.outer_boundary().area();
        return gmpq_to_double(and_area / or_area);
    }

    size_t size() const
    {
        return poly.size();
    }

};
}

#endif // SOLVER_ORIGAMI_HPP
