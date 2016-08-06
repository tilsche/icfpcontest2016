#pragma once

#include <set>
#include <vector>

#include <zebra/geometry.hpp>
#include <zebra/node_graph.hpp>
#include <zebra/solution.hpp>
#include <zebra/solver.hpp>

namespace zebra
{
    class backward : public solver
    {
    public:
        solution operator()(task t) override;

        void transitive_hull(point begin, point end, std::vector<point>& stack, std::vector<std::set<point>>& ret);

    private:
        node_graph ngraph;
    };

}
