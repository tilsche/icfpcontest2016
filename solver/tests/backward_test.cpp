#include <zebra/geometry.hpp>
#include <zebra/task.hpp>
#include <zebra/node_graph.hpp>
#include <zebra/solvers/solve_backward.hpp>

using namespace zebra;

const std::string filename = "../../tasks/task009.txt";

class TestTask{

public:
    TestTask()
    :m_task(zebra::read_task(filename)),
     m_graph(m_task),
     m_possible_paths(0)
    {
        solution s = m_bsolver(m_task);
        m_possible_paths.push_back(std::set<point>{point_from_string("0,0"), point_from_string("1/2, 0")});
        m_possible_paths.push_back(std::set<point>{point_from_string("0,2/3"), point_from_string("1/2, 2/3")});
    }

    task m_task;
    node_graph m_graph;
    backward m_bsolver;
    std::vector<std::set<point>> m_possible_paths;
};

int main(void)
{
    TestTask tt();

    return 0;
}
