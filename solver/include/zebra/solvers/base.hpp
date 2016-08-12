
#ifndef SOLVER_SOLVER_HPP
#define SOLVER_SOLVER_HPP

#include <zebra/solution.hpp>
#include <zebra/task.hpp>

namespace zebra
{
namespace solver
{
    using clock = std::chrono::system_clock;

    class base
    {
    public:
        virtual ~base()
        {
        }

        virtual solution operator()(const task&) = 0;
    };
}
}
#endif // SOLVER_SOLVER_HPP
