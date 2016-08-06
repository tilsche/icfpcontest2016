
#ifndef SOLVER_ORIGAMI_HPP
#define SOLVER_ORIGAMI_HPP

#include <zebra/geometry.hpp>
#include <zebra/log.hpp>
#include <zebra/solution.hpp>

#include <CGAL/Boolean_set_operations_2.h>

namespace zebra
{
class origami
{
public:
    solution sol = solution::unit_square();

    std::vector<origami> move_to(point p) const
    {
        logging::debug() << "MOVE_TO (" << p << ")[[[[\n" << sol << "\n]]]]] MOVE_TO";
        std::vector<origami> results;
        results.reserve(sol.destination_positions.size() * 2);
        for (auto dp : sol.destination_positions)
        {
            transformation move(CGAL::TRANSLATION, p - dp);
            auto moved = *this;
            moved.sol.transform(move);
            logging::trace() << "MOVED (" << dp << ")[[[[\n" << moved.sol << "\n]]]]] MOVED";

            results.push_back(moved);
        }
        return results;
    }o
};
}

#endif // SOLVER_ORIGAMI_HPP
