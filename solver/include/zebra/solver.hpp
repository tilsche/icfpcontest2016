
#ifndef SOLVER_SOLVER_HPP
#define SOLVER_SOLVER_HPP

#include <zebra/solution.hpp>
#include <zebra/task.hpp>

namespace zebra
{

class solver
{
public:
    virtual ~solver()
    {
    }

    virtual solution operator()(task) = 0;
};
}
#endif // SOLVER_SOLVER_HPP
