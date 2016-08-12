
#ifndef SOLVER_SIMPLE_HPP
#define SOLVER_SIMPLE_HPP

#include <zebra/solvers/base.hpp>

#include <zebra/log.hpp>
#include <zebra/origami.hpp>

namespace zebra
{
namespace solver
{
    class simple : public base
    {
    public:
        solution operator()(const task& t) override
        {
            logging::info() << "Simple solver starting..";

            polygon target = t.sil.shape().outer_boundary();
            assert(target.is_counterclockwise_oriented());

            origami ori;

            double best_r = 0;
            origami best_ori;
            for (auto vx_it = target.vertices_begin(); vx_it != target.vertices_end(); vx_it++)
            {
                for (auto o2 : ori.move_to(*vx_it))
                {
                    auto new_r = o2.sol.resemblance(target);
                    if (new_r > best_r)
                    {
                        best_r = new_r;
                        best_ori = o2;
                    }
                }
            }
            logging::info() << "Simple found best resemblence " << best_r;

            return best_ori.sol;
        }
    };
}
}
#endif // SOLVER_SIMPLE_HPP
