#include <zebra/geometry.hpp>
#include <zebra/task.hpp>

#include <iostream>

namespace zebra
{
void check_skeleton(const std::string& taskname)
{
    auto t = read_task(taskname);

    auto check = [&](point p) {
        if (t.sil.shape().outer_boundary().has_on_unbounded_side(p)) {
            std::cout << taskname << ": skeleton point on unbounded side" << std::endl;
        }
    };

    for (const auto& edge : t.skel.edges)
    {
        check(edge.source());
        check(edge.target());
    }
}
}

int main(int argc, const char* argv[])
{
    assert(argc == 2);
    zebra::check_skeleton(argv[1]);
}

