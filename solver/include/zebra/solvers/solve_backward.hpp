#ifndef SOLVER_BACKWARD_HPP
#define SOLVER_BACKWARD_HPP

#include <set>

#include <zebra/geometry.hpp>
#include <zebra/log.hpp>
#include <zebra/node_graph.hpp>
#include <zebra/solution.hpp>
#include <zebra/solver.hpp>

namespace zebra
{
    class backward : public solver
    {
    public:
        solution operator()(task t) override;

        std::set<point> transitive_hull(point begin, point end);

    private:
        node_graph ngraph;

    };

}

#endif /* SOLVER_BACKWARD_HPP */
