
#ifndef SOLVER_STUPIDSOLVE_HPP
#define SOLVER_STUPIDSOLVE_HPP

#include <zebra/solvers/base.hpp>

namespace zebra
{
namespace solver
{
    class stupid : public base
    {
    public:
        solution operator()(const task&) override
        {
            origami o;
            return o.sol;
        }
    };
}
}
#endif // SOLVER_STUPIDSOLVE_HPP
