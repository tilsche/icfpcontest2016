#include <zebra/log.hpp>
#include <zebra/solution.hpp>

namespace zebra
{
void test_rotate1()
{
    solution s = solution::unit_square();
    auto fold1 = line_segment_from_string("0,0 1/3,1/4");
    auto point1 = point_from_string("0,0");
    logging::info() << "Testing to rotate:\n" << s;
    logging::info() << "with line segment " << fold1;
    logging::info() << "around point " << point1;

    s.rotate(point1, fold1);

    s.to_png(std::string(__func__) + "_post1");
    logging::info() << "Result:\n" << s;

    logging::debug() << s.get_facet(0).transform();

    auto fold2 = line_segment_from_string("0,0 1/3,-1/4");
    auto point2 = point_from_string("0,0");
    logging::info() << "Testing to rotate:\n" << s;
    logging::info() << "with line segment " << fold2;
    logging::info() << "around point " << point2;

    s.rotate(point2, fold2);

    s.to_png(std::string(__func__) + "_post2");
    logging::info() << "Result:\n" << s;

    logging::debug() << s.get_facet(0).transform();
}

void test_rotate2()
{
    solution s = solution::unit_square();

    auto fold1 = line_segment_from_string("0,0 1/3,1/4");
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

    auto fold1 = line_segment_from_string("0,0 1/3,1/4");
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

    auto fold1 = line_segment_from_string("0,0 1/3,1/4");
    auto point1 = point_from_string("0,0");
    logging::info() << "Testing to rotate:\n" << s;
    logging::info() << "with line segment " << fold1;
    logging::info() << "around point " << point1;

    s.rotate(point1, fold1);

    s.to_png(std::string(__func__) + "_post1");
    logging::info() << "Result:\n" << s;
}

void test_rotate5()
{
    solution s = solution::unit_square();

    auto fold1 = line_segment_from_string("0,0 0,1");
    auto point1 = point_from_string("0,0");
    logging::info() << "Testing to rotate:\n" << s;
    logging::info() << "with line segment " << fold1;
    logging::info() << "around point " << point1;

    s.rotate(point1, fold1);

    s.to_png(std::string(__func__) + "_post1");
    logging::info() << "Result:\n" << s;

    auto fold2 = line_segment_from_string("0,0 0,-1");
    auto point2 = point_from_string("0,0");
    logging::info() << "Testing to rotate:\n" << s;
    logging::info() << "with line segment " << fold2;
    logging::info() << "around point " << point2;

    s.rotate(point2, fold2);

    s.to_png(std::string(__func__) + "_post2");
    logging::info() << "Result:\n" << s;
}

void test_rotate6()
{
    solution s = solution::unit_square();

    auto fold1 = line_segment_from_string("0,0 1/3,-1/4");
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
    zebra::test_rotate5();
    zebra::test_rotate6();
}
