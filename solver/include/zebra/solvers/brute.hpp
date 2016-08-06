
#ifndef SOLVER_BRUTESOLVER_HPP
#define SOLVER_BRUTESOLVER_HPP

#include <zebra/solver.hpp>

#include <zebra/log.hpp>
#include <zebra/origami.hpp>

#include <exception>

namespace zebra
{
class brutesolver : public solver
{
    class solution_found : public std::exception
    {
    };

    class state
    {
    public:
        state(const task& tt, int md = 2) : t(tt), max_depth(md)
        {
        }

        // returns true if you should break;
        bool check(const origami& o, int depth)
        {
            auto new_r = o.sol.resemblance(t.sil.shape());
            logging::debug() << "[[[CHECKING at depth " << depth << " r " << new_r;
            logging::debug() << o.sol << "\n]]]";
            if (new_r > best_resemblance)
            {
                best_resemblance = new_r;
                best_solution = o.sol;
                if (new_r == 1.0)
                {
                    throw solution_found();
                }
            }
            if (depth > max_depth)
            {
                return true;
            }
            return false;
        }

        void fold_recurse(const origami& o, int depth = 0)
        {
            if (check(o, depth))
            {
                return;
            }
            for (auto line : t.skel.lines())
            {
                auto o2 = o;
                o2.sol.fold(line);
                fold_recurse(o2, depth + 1);
            }
        }

        void move_recurse(const origami& o, int depth = 0)
        {
            if (check(o, depth))
            {
                return;
            }
            auto bound = t.sil.shape().outer_boundary();
            for (auto vx_it = bound.vertices_begin(); vx_it != bound.vertices_end(); vx_it++)
            {
                for (auto o2 : o.move_to(*vx_it))
                {
                    fold_recurse(o2, depth + 1);
                }
            }
        }

        const task& t;
        solution best_solution;
        double best_resemblance = 0;
        int max_depth;
    };

public:
    solution operator()(const task& t) override
    {
        logging::info() << "Brute force solver starting..";

        origami ori;

        state st(t);
        try
        {
            st.move_recurse(ori);
        }
        catch (solution_found&)
        {
            logging::info() << "PERFECT SOLUTION FOUND";
        }
        logging::info() << "Best solution: " << st.best_resemblance;
        return st.best_solution;
    }
};
}
#endif // SOLVER_BRUTESOLVER_HPP
