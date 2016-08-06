
#ifndef SOLVER_STUPIDSOLVE_HPP
#define SOLVER_STUPIDSOLVE_HPP

#include <zebra/solver.hpp>

namespace zebra
{
class stupid : public solver
{
public:
    solution operator()(const task&) override
    {
        origami o;
        return o.sol;
    }
};
}
#endif // SOLVER_STUPIDSOLVE_HPP
