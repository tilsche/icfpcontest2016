
#ifndef SOLVER_STUPIDSOLVE_HPP
#define SOLVER_STUPIDSOLVE_HPP

#include <zebra/solver.hpp>

namespace zebra
{
class stupid : public solver
{
public:
    solution operator()(task t) override
    {
        solution s;
        s.source_positions = make_positions_1();
        s.destination_positions = make_positions_1();
        s.facets.push_back({ 0, 1, 2, 3 });
        return s;
    }
};
}
#endif // SOLVER_STUPIDSOLVE_HPP
