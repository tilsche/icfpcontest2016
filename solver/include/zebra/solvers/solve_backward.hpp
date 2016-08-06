#pragma once

#include <set>
#include <vector>

#include <zebra/geometry.hpp>
#include <zebra/node_graph.hpp>
#include <zebra/solution.hpp>
#include <zebra/solver.hpp>

namespace zebra
{
    using hull = std::set<point>;
    using hull_list = std::vector<hull>;
    
    class backward : public solver
    {
    public:
        solution operator()(task t) override;

        void transitive_hull(point begin, point end, std::vector<point>& stack, hull_list& ret);

    private:
        node_graph ngraph;

        void transitive_hull_in(point begin, point end, std::vector<point>& stack, hull_list& ret);
    };

}
