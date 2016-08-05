#ifndef SOLVER_BACKWARD_HPP
#define SOLVER_BACKWARD_HPP

#include <zebra/log.hpp>
#include <zebra/node_graph.hpp>

namespace zebra {

class backward : public solver
{
    public:
        solution operator()(task t) override
        {
            logging::info() << "backward solver starting..";
            solution s; //TODO
            node_graph ngraph(t);

        }

};

}

#endif /* SOLVER_BACKWARD_HPP */
