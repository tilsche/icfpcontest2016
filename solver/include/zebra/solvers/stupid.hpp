
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
        polygon one;
        one.push_back({ 0, 0 });
        one.push_back({ 1, 0 });
        one.push_back({ 1, 1 });
        one.push_back({ 0, 1 });
        s.source_positions = one;
        s.destination_positions = one;
        s.facets.push_back({ 0, 1, 2, 3 });
        return s;
    }
};
}
#endif // SOLVER_STUPIDSOLVE_HPP
