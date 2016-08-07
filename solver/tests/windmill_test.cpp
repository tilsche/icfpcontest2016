#include <zebra/log.hpp>
#include <zebra/solution.hpp>

namespace zebra
{
void test_windmill()
{
    solution s = solution::unit_square();

    auto fold1 = line_segment_from_string("3/4,0 3/4,1");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold1;

    s.fold(0, fold1);

    s.to_png(std::string(__func__) + "_post1");
    logging::info() << "Result:\n" << s;

    auto fold2 = line_segment_from_string("1,3/4 0,3/4");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold2;

    s.fold(0, fold2);
    s.to_png(std::string(__func__) + "_post2");
    logging::info() << "Result:\n" << s;

    assert(s.vertex_size() == 9);

    auto fold3 = line_segment_from_string("0,0 1,1");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold3;

    s.fold(2, fold3);
    s.to_png(std::string(__func__) + "_post3");
    logging::info() << "Result:\n" << s;

    auto fold4 = line_segment_from_string("0,3/4 1,3/4");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold4;

    s.fold(3, fold4);
    s.to_png(std::string(__func__) + "_post4");
    logging::info() << "Result:\n" << s;

    auto fold5 = line_segment_from_string("3/4,3/4 0,-1/2");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold5;

    s.fold(2, fold5);
    s.to_png(std::string(__func__) + "_post5");
    logging::info() << "Result:\n" << s;
}
}

int main()
{
    zebra::test_windmill();
}
