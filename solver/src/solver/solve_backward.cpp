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
        std::vector<node_graph> res({ngraph});

        for(unsigned int i = 0; i < res.size(); ++i)
        {
            unfold_segments(res[i], res);
        logging::info() << "res size" << res.size();
        }

        return s;
    }

    void backward::unfold_segments(node_graph ng, std::vector<node_graph>& result)
    {
        std::set<std::set<upoint>> ultraset;

        for (const auto& kvp : ng)
        {
            for (const auto& elem : kvp.second)
            {
                ultraset.insert({kvp.first, elem});
            }
        }

        for (const auto& line : ultraset)
        {
            assert(line.size() == 2);

            auto ultraiter = line.begin();
            auto begin = *ultraiter;
            auto end = *(++ultraiter);
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
                auto new_graph = ng;
                std::vector<upoint> poly_points;
                for(const auto& p : hull)
                {
                    // Calc transformation parameters
                    upoint beg2point(p.x() - begin.x(), p.y() - begin.y());
                    auto dist = CGAL::to_double(beg2point.x() * normal.x() + beg2point.y() * normal.y())
                            / sqrt(CGAL::to_double(normal.squared_length()));

                    // Build tranlation matrix
                    vector translate_vec = (-2 * dist) * normal;
                    CGAL::Gmpq zero(0,0);
                    transformation translate(zero, zero, translate_vec.hx(),
                                zero, zero, translate_vec.hy(), translate_vec.hw());

                    // Move point
                    auto p_new = translate(p);
                    poly_points.push_back(p_new);

                    // Change point in new node_graph
                    new_graph[p_new] = ng[p];
                    for(const auto& p : new_graph[p_new])
                    {
                        new_graph[p].erase(p);
                        new_graph[p].insert(p_new);
                    }
                }
                poly_points.push_back(begin);
                poly_points.push_back(end);
                new_graph.m_polys.emplace_back(poly_points.begin(), poly_points.end());
                if (std::find(result.begin(), result.end(), new_graph) == result.end()
                 && BackwardConstraints::test(new_graph))
                {
                    result.push_back(new_graph);
                }
            }
        }
    }

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
