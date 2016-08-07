#include <zebra/log.hpp>
#include <zebra/solution.hpp>

namespace zebra
{
void test_rotate1()
{
    solution s = solution::unit_square();

    auto fold1 = line_segment_from_string("0,0 1,1/2");
    auto point1 = point_from_string("0,0");
    logging::info() << "Testing to rotate:\n" << s;
    logging::info() << "with line segment " << fold1;
    logging::info() << "around point " << point1;

    s.rotate(point1, fold1);

    s.to_png(std::string(__func__) + "_post1");
    logging::info() << "Result:\n" << s;
}

void test_rotate2()
{
    solution s = solution::unit_square();

    auto fold1 = line_segment_from_string("0,0 1,1/2");
    auto point1 = point_from_string("1,0");
    logging::info() << "Testing to rotate:\n" << s;
    logging::info() << "with line segment " << fold1;
    logging::info() << "around point " << point1;

    s.rotate(point1, fold1);

    s.to_png(std::string(__func__) + "_post1");
    logging::info() << "Result:\n" << s;
}

void test_rotate3()
{
    solution s = solution::unit_square();

    auto fold1 = line_segment_from_string("0,0 1,1/2");
    auto point1 = point_from_string("1/2,1/2");
    logging::info() << "Testing to rotate:\n" << s;
    logging::info() << "with line segment " << fold1;
    logging::info() << "around point " << point1;

    s.rotate(point1, fold1);

    s.to_png(std::string(__func__) + "_post1");
    logging::info() << "Result:\n" << s;
}

void test_rotate4()
{
    solution s = solution::unit_square();

    auto fold1 = line_segment_from_string("0,0 1,-1/2");
    auto point1 = point_from_string("0,0");
    logging::info() << "Testing to rotate:\n" << s;
    logging::info() << "with line segment " << fold1;
    logging::info() << "around point " << point1;

    s.rotate(point1, fold1);

    s.to_png(std::string(__func__) + "_post1");
    logging::info() << "Result:\n" << s;
}
}

int main()
{
    zebra::test_rotate1();
    zebra::test_rotate2();
    zebra::test_rotate3();
    zebra::test_rotate4();
}
