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
        std::vector<std::set<point>> ret;

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
                                   std::vector<std::set<point>>& ret)
    {
        if(stack.size() <= ngraph.size())
        {
            for(const auto& elem : ngraph[begin])
            {
                stack.push_back(elem);
                if(elem == end)
                {
                    stack.pop_back();
                    std::set<point> set;
                    for(const auto stack_elem : stack)
                    {
                        set.insert(stack_elem);
                    }
                    ret.push_back(set);
                }
                else if(std::find(stack.begin(), stack.end(), begin) != stack.end())
                {
                    stack.pop_back();
                }
                else
                {
                    transitive_hull(elem, end, stack, ret);
                    stack.pop_back();
                }
            }
        }
    }
}
