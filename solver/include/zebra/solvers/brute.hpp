
#ifndef SOLVER_BRUTESOLVER_HPP
#define SOLVER_BRUTESOLVER_HPP

#include <zebra/solver.hpp>

#include <zebra/log.hpp>
#include <zebra/origami.hpp>

#include <chrono>
#include <exception>
#include <iostream>

namespace zebra
{
class brutesolver : public solver
{
    using clock = std::chrono::system_clock;

    class solution_found : public std::exception
    {
    };
    class timeout : public std::exception
    {
    };

    class state
    {
    public:
        state(const task& tt, int md, clock::time_point dl) : t(tt), max_depth(md), deadline(dl)
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
            if (clock::now() > deadline)
            {
                throw timeout();
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
            for (auto l : t.skel.unique_lines())
            {
                logging::debug() << "{{" << depth << "}} next line " << l;
                for (auto o2 : o.folds(l))
                {
                    fold_recurse(o2, depth + 1);
                }
                logging::debug() << "{{" << depth << "}} next line " << l.opposite();
                for (auto o2 : o.folds(l.opposite()))
                {
                    fold_recurse(o2, depth + 1);
                }
            }
        }

        void align_recurse(const origami& o, int depth = 0)
        {
            if (check(o, depth))
            {
                return;
            }
            for (const auto& ls : t.skel.edges)
            {
                for (auto o2 : o.align_to(ls))
                {
                    fold_recurse(o2, depth + 1);
                }
            }
        }
        const task& t;
        solution best_solution;
        double best_resemblance = 0;
        int max_depth;
        clock::time_point deadline;
    };

    clock::time_point deadline;

public:
    template <class D>
    brutesolver(D timelimit)
    {
        auto start = clock::now();
        deadline = start + timelimit / 2;
    }

    solution operator()(const task& t) override
    {
        origami ori;

        for (int rec = 1;; rec++)
        {
            logging::info() << "Brute forcing recursion: " << rec;
            state st(t, rec, deadline);
            try
            {
                st.align_recurse(ori);
            }
            catch (solution_found&)
            {
                logging::info() << "PERFECT SOLUTION FOUND";
                std::cout << st.best_resemblance << std::endl;
                return st.best_solution;
            }
            catch (timeout&)
            {
                logging::info() << "TIMEOUT";
                std::cout << st.best_resemblance << std::endl;
                return st.best_solution;
            }
        }
    }
};
}
#endif // SOLVER_BRUTESOLVER_HPP
