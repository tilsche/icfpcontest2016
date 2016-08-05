#include <zebra/task.hpp>
#include <cassert.h>

using namespace zebra;

int main()
{
    std::string testfile {"../tasks/task001.txt"};
    auto t1 = zebra::read_task(testfile);

    assert(t1.polygons.size() == 1);
    assert(t1.edges.size() == 4)

    return 0;
}
