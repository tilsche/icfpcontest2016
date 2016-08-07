#include <zebra/log.hpp>
#include <zebra/solution.hpp>

namespace zebra
{
void test_rotate()
{
    solution s = solution::unit_square();

    auto fold1 = line_segment_from_string("0,0 1,1/2");
    logging::info() << "Testing to rotate:\n" << s;
    logging::info() << "with line segment " << fold1;

    s.rotate(fold1);

    s.to_png(std::string(__func__) + "_post1");
    logging::info() << "Result:\n" << s;
}
}

int main()
{
    zebra::test_rotate();
}
