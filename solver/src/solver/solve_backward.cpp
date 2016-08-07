#include <CGAL/squared_distance_2.h>
#include <cmath>
#include <set>
#include <vector>

#include <zebra/solvers/backward_constraints.hpp>
#include <zebra/solvers/solve_backward.hpp>
#include <zebra/log.hpp>

namespace zebra
{
    solution backward::operator()(const task& t)
    {
        logging::info() << "backward solver starting..";
        solution s; //TODO
        ngraph = node_graph(t);

        point ps(0, 0);
        point pe(1, 1);
        std::cout << BackwardConstraints::valid_length(ps, pe) << '\n';
        std::cout << BackwardConstraints::is_standard_square(ngraph) << '\n';

        return s;
    }

    //std::set<node_graph> backward::unfold_segment(node_graph ng)
    void backward::unfold_segment(upoint begin, upoint end)
    {
        //for
        // Get hulls
        std::vector<upoint> stack;
        hull_list h_list;

        transitive_hull(begin, end, stack, h_list);

        // Get normal
        line_segment seg( begin, end );
        auto dir = seg.direction();
        kernel::Vector_2 normal(-dir.dy(), dir.dx());

        for(const auto& hull : h_list)
        {
            for(const auto& p : hull)
            {
                // Calc transformation parameters
                upoint beg2point(p.x() - begin.x(), p.y() - begin.y());
                auto dist = CGAL::to_double(beg2point.x() * normal.x() + beg2point.y() * normal.y()) / sqrt(CGAL::to_double(normal.squared_length()));

                // Build tranlation matrix
                vector translate_vec = (-2 * dist) * normal;
                CGAL::Gmpq zero(0,0);
                transformation translate(zero, zero, translate_vec.hx(), zero, zero, translate_vec.hy(), translate_vec.hw());

                // Move point
                auto p_new = translate(p);

                // Change point in new node_graph
                auto new_graph = ngraph;
                new_graph[p_new] = ngraph[p];
                for(const auto& p : new_graph[p_new])
                {
                    new_graph[p].erase(p);
                    new_graph[p].insert(p_new);
                }
            }
        }
    }

    //void backward::make_node_connections_unique(node_graph& ng)
    //{
        //for(auto&
    //}

    void backward::transitive_hull(upoint begin,
                                   upoint end,
                                   std::vector<upoint>& stack,
                                   hull_list& ret)
    {
        stack.push_back(begin);
        transitive_hull_in(begin, end, stack, ret);
        for(auto& elem : ret)
        {
            elem.erase(begin);
            if(elem.size() == 0)
            {
                auto it = std::find(ret.begin(), ret.end(), elem);
                ret.erase(it);
            }
        }
    }

    void backward::transitive_hull_in(upoint begin,
                                      upoint end,
                                      std::vector<upoint>& stack,
                                      hull_list& ret)
    {
        if(stack.size() <= ngraph.size())
        {
            for(const auto& elem : ngraph[begin])
            {
                if(elem == end)
                {
                    std::set<upoint> set;
                    for(const auto stack_elem : stack)
                    {
                        set.insert(stack_elem);
                    }
                    ret.push_back(set);
                }
                else if(std::find(stack.begin(), stack.end(), elem) != stack.end())
                {
                    continue;
                }
                else
                {
                    stack.push_back(elem);
                    transitive_hull_in(elem, end, stack, ret);
                    stack.pop_back();
                }
            }
        }
    }
}
