#ifndef SOLVER_BFS_HPP
#define SOLVER_BFS_HPP

#include <zebra/solver.hpp>

#include <zebra/log.hpp>
#include <zebra/origami.hpp>

#include <chrono>
#include <exception>
#include <iostream>

namespace zebra
{
class bfs : public solver
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
        state(const task& tt, clock::time_point dl) : t(tt), deadline(dl)
        {
            target_area = tt.sil.shape().outer_boundary().area().to_double();
        }

        ~state()
        {
            logging::info() << "Total solver checks: " << check_count;
        }

        struct element
        {
            origami o;
            double res;
            CGAL::Gmpq area;
            int depth = 0;

            bool operator<(const element& other) const
            {
                return res < other.res;
            }
        };

        void check(const element& e)
        {
            check_count++;
            if (e.res > best_resemblance)
            {
                best_resemblance = e.res;
                best_solution = e.o.sol;
                if (e.res == 1.0)
                {
                    throw solution_found();
                }
            }
            if (clock::now() > deadline)
            {
                throw timeout();
            }
        }

        std::priority_queue<element> q_;

        void queue(const element& e)
        {
            check(e);
            q_.emplace(e);
        }

        void queue(const origami& o)
        {
            auto p = o.sol.poly();
            queue(o, o.sol.resemblance(t.sil.shape()), holy_area(p));
        }

        void queue(const origami& o, double r, CGAL::Gmpq a)
        {
            queue(element{ o, r, a });
        }

        void queue_align(const origami& o)
        {

            std::vector<polygon_with_holes> visited;
            for (const auto& ls : t.skel.edges)
            {
                for (auto o2 : o.align_to(ls))
                {
                    auto p = o2.sol.poly();
                    if (visited.end() != std::find(visited.begin(), visited.end(), p))
                    {
                        logging::debug() << "Skipping redundant move";
                        continue;
                    }
                    visited.emplace_back(p);
                    auto res = o2.sol.resemblance(t.sil.shape());
                    if (res > 0.0)
                    {
                        logging::debug() << "**** Trying to align at " << ls << " resemb " << res;
                        queue(o2);
                    }
                    else
                    {
                        logging::debug() << "**** skipping useless alignment at " << ls;
                    }
                }
            }
            queue(o);
        }

        void queue_fold()
        {
            auto elem = q_.top();
            q_.pop();

            const auto& o = elem.o;
            auto old_area = elem.area;
            for (auto l : t.skel.unique_lines())
            {
                logging::debug() << " next line " << l;
                for (auto o2 : o.folds(l))
                {
                    auto new_area = holy_area(o2.sol.poly());
                    if (new_area < old_area)
                    {
                        queue(o2);
                    }
                }
            }
        }

        const task& t;
        solution best_solution;
        double target_area;
        double best_area_error = 999999.0;
        double best_resemblance = 0;
        int max_depth;
        clock::time_point deadline;
        size_t check_count = 0;
    };

    clock::time_point deadline;

public:
    template <class D>
    bfs(D timelimit)
    {
        auto start = clock::now();
        deadline = start + timelimit;
    }

    solution operator()(const task& t) override
    {
        origami ori;

        logging::info() << "BFS starting";

        state st(t, deadline);
        try
        {
            st.queue_align(ori);
            while (true)
            {
                st.queue_fold();
            }
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
};
}
#endif // SOLVER_BFS_HPP
