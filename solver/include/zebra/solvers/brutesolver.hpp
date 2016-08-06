
#ifndef SOLVER_BRUTESOLVER_HPP
#define SOLVER_BRUTESOLVER_HPP

#include <zebra/solver.hpp>

#include <zebra/log.hpp>
#include <zebra/origami.hpp>

namespace zebra
{
class brutesolver : public solver
{
    class state
    {
    public:
        state(const task& tt, int md = 5) : t(tt), max_depth(5);
        {
        }

        void fold_recurse(const origami& o, int depth = 0)
        {
        }

        void move_recurse(const origami& o, int depth = 0)
        {
            auto bound = t.sil.shape().outer_boundary();
            for (auto vx_it = bound.vertices_begin(); vx_it = bound.vertices_end(); vx_it++)
            {
                for (auto o2 : o.move_to(*vx_it))
                {
                    fold_recurse(o2, depth + 1);
                    /*                auto new_r = o2.resemblance(target);
                                    if (new_r > best_r)
                                    {
                                        best_r = new_r;
                                        best_ori = o2;
                                    }
                                    */
                }
            }
        }

        const task& t;
        int max_depth;
    };

public:
    solution operator()(const task& t) override
    {
        logging::info() << "Brute force solver starting..";

        origami ori;

        state st(t);
        st.move_recurse(ori);
        /*
        move double best_r = 0;
        origami best_ori;
        for (auto vx_it = target.vertices_begin(); vx_it != target.vertices_end(); vx_it++)
        {
            for (auto o2 : ori.move_to(*vx_it))
            {
                auto new_r = o2.resemblance(target);
                if (new_r > best_r)
                {
                    best_r = new_r;
                    best_ori = o2;
                }
            }
        }
        auto vector = best_ori.poly.vertex(0) - ori.poly.vertex(0);
        logging::info() << "Simple found best resemblence " << best_r
                        << " vector: " << gmpq_to_string(vector.hx()) << ", "
                        << gmpq_to_string(vector.hy());

        transformation move(CGAL::TRANSLATION, vector);
        s.destination_positions = make_positions_1();
        for (auto& destination_position : s.destination_positions)
        {
            destination_position = move(destination_position);
        }
        return s;*/
        return ori.sol;
    }
};
}
#endif // SOLVER_BRUTESOLVER_HPP
