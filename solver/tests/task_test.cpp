#include <zebra/task.hpp>
#include <cassert>

using namespace zebra;

int main(int argc, const char** argv)
{
    std::string testfile {argv[1]};
    auto t1 = zebra::read_task(testfile);

    assert(t1.sil.polygons.size() == 1);
    assert(t1.skel.edges.size() == 4);

    return 0;
}
