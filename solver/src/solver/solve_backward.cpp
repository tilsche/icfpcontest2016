#include <set>

#include <zebra/solvers/solve_backward.hpp>
#include <zebra/log.hpp>

namespace zebra
{
    solution backward::operator()(task t)
    {
        logging::info() << "backward solver starting..";
        solution s; //TODO
        ngraph = node_graph(t);
        
        return s;
    }

    std::set<point> backward::transitive_hull(point begin, point end)
    {
        return std::set<point>();
    }
}
