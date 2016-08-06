#include <zebra/solvers/solve_backward.hpp>

namespace zebra
{
    solution backward::operator()(task t)
    {
        logging::info() << "backward solver starting..";
        solution s; //TODO
        ngraph = node_graph(t);
    }

    std::set<point> backward::transitive_hull(point begin, point end)
    {
        return std::set<point>();
    }
}
