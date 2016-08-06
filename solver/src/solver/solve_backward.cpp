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
        
        transitive_hull((*(ngraph.m_graph.begin())).first, (*(ngraph.m_graph.begin()++)).first);
        
        return s;
    }

    std::set<point> backward::transitive_hull(point begin, point end)
    {
        if( begin == end )
        {
            return std::set<point>();
        }
        
        auto ret_set = std::set<point>();
        for(const auto& elem : ngraph[begin])
        {
            auto ret = transitive_hull(elem, end);
            ret_set.insert(ret.begin(), ret.end());
        }
        
        logging::info() << "hull:\n\t";
        for(const auto& elem : ret_set)
        {
            logging::info() << point_to_string(elem) << "\n\t";
        }
        logging::info() << "\n\n";

        return ret_set;
    }
}
