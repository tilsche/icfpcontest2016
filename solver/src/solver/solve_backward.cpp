#include <set>
#include <vector>

#include <zebra/solvers/backward_constraints.hpp>
#include <zebra/solvers/solve_backward.hpp>
#include <zebra/log.hpp>

namespace zebra
{
    solution backward::operator()(task t)
    {
        logging::info() << "backward solver starting..";
        solution s; //TODO
        ngraph = node_graph(t);
        
        std::vector<point> stack;
        hull_list ret;

        logging::info() << "backward before";
        transitive_hull(point_from_string("0,0"), point_from_string("1/2,2/3"), stack, ret);

        point ps(0, 0);
        point pe(1, 1);
        std::cout << BackwardConstraints::valid_length(ps, pe) << '\n';
        std::cout << BackwardConstraints::is_standard_square(ngraph) << '\n';
        
        for(const auto& elem : ret)
        {
            logging::info() << "hull";
            for(const auto& i : elem)
            {
                logging::info() << "\t" << point_to_string(i);
            }
        }
        
        return s;
    }

    void backward::transitive_hull(point begin,
                                   point end,
                                   std::vector<point>& stack,
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

    void backward::transitive_hull_in(point begin,
                                      point end,
                                      std::vector<point>& stack,
                                      hull_list& ret)
    {
        if(stack.size() <= ngraph.size())
        {
            for(const auto& elem : ngraph[begin])
            {
                if(elem == end)
                {
                    std::set<point> set;
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
