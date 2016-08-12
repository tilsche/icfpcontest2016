#ifndef SOLVER_META_HPP
#define SOLVER_META_HPP

#include "bfs.hpp"
#include "brute.hpp"
#include "simple.hpp"
#include "stupid.hpp"

#include <zebra/log.hpp>
#include <zebra/solvers/base.hpp>

#include <exception>
#include <memory>

namespace zebra
{
namespace solver
{
    class meta : public base
    {
    public:
        template <class D>
        meta(const std::string& name, D timelimit)
        {
            if (name == "stupid")
            {
                backend_ = std::make_unique<solver::stupid>();
            }
            else if (name == "simple")
            {
                backend_ = std::make_unique<solver::simple>();
            }
            else if (name == "brute")
            {
                backend_ = std::make_unique<solver::brute>(timelimit);
            }
            else if (name == "bfs")
            {
                backend_ = std::make_unique<solver::bfs>(timelimit);
            }
            else
            {
                throw std::runtime_error("unknown solver " + name);
            }
            logging::info() << "Initialized " << name << " solver";
        }

        solution operator()(const task& t) override
        {
            auto t0 = clock::now();
            auto s = (*backend_)(t);
            auto t1 = clock::now();
            logging::info()
                << "Solver took "
                << std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count()
                << " seconds";
            return s;
        }

    private:
        std::unique_ptr<base> backend_;
    };
}
}
#endif // SOLVER_META_HPP
