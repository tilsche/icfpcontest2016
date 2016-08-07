#pragma once

#include <set>
#include <vector>

#include <zebra/geometry.hpp>
#include <zebra/node_graph.hpp>
#include <zebra/solution.hpp>
#include <zebra/solver.hpp>

namespace zebra
{
    using hull = std::set<upoint>;
    using hull_list = std::vector<hull>;

    class backward : public solver
    {
    public:
        solution operator()(const task& t) override;
        std::set<node_graph> unfold_segments(node_graph ng);

    private:
        node_graph ngraph;

        void transitive_hull(upoint begin, upoint end, std::vector<upoint>& stack, hull_list& ret);
        void transitive_hull_in(upoint begin, upoint end, std::vector<upoint>& stack, hull_list& ret);
    };

}
