#include <zebra/solution.hpp>
using namespace std;
using namespace zebra;

void test1();
void test2();
void test3();
void testmulti();
void test1c();
void test2c();
void testedge();
void testcross();
void test1c2();

int main()
{
    // test1();
    // test2();
    // test3();
    // testmulti();

    // test1c();
    // test2c();
    // testedge();
    // testcross();
    test1c2();
    return 0;
}

void test1()
{
    // cerr << __func__ << endl;

    // +---+
    // |   |
    // |   |
    // |   |
    // +---+

    solution s = solution::unit_square();

    line_segment fold = line_segment_from_string("1/2,0 1/2,1");

    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold;

    s.fold(fold);

    s.to_png(string(__func__) + "_post");

    logging::info() << "Result:\n" << s;

    //   +-+
    //   | |
    //   | |
    //   | |
    //   +-+

    // TODO check

    // assert(s.facets_.size() == 2);
}

void test2()
{
    // cerr << __func__ << endl;

    // +---+
    // |   |
    // |   |
    // |   |
    // +---+

    solution s = solution::unit_square();

    auto fold = line_segment_from_string("0,1/2 1,3/4");

    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold;

    s.fold(fold);

    s.to_png(string(__func__) + "_post");
    logging::info() << "Result:\n" << s;
    //
    //
    // +---+
    // |   |
    // +---+

    // TODO check
    // assert(s.facets_.size() == 2);
}

void test3()
{
    // cerr << __func__ << endl;

    // +---------+
    // |         |
    // +---------+
    // |         |
    // +---------+

    solution s;
    s.source_positions_ = {
        { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { 1, CGAL::Gmpq(1, 2) }, { 0, CGAL::Gmpq(1, 2) }
    };

    s.facets_ = { facet{.vertex_ids_ = { 0, 1, 4, 5 } }, facet{.vertex_ids_ = { 5, 4, 2, 3 } } };

    //
    //
    // +---------+
    // |         |
    // +---------+
    s.destination_positions_ = {
        { 0, 0 }, { 1, 0 }, { 1, 0 }, { 0, 0 }, { 1, CGAL::Gmpq(1, 2) }, { 0, CGAL::Gmpq(1, 2) }
    };

    auto fold = line_segment_from_string("1/2,0 1/2,1");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold;

    s.fold(fold);

    s.to_png(string(__func__) + "_post");
    logging::info() << "Result:\n" << s;
    //
    //
    //    +--+
    //    |  |
    //    +--+

    // TODO check
    // assert(s.facets_.size() == 4);
}

void testmulti()
{
    // cerr << __func__ << endl;

    //   +---------+
    //   |         |
    //   +---------+
    //   |         |
    //   +---------+
    //   |         |
    //   +---------+
    //   |         |
    // 0 +---------+

    solution s = solution::unit_square();

    auto fold1 = line_segment_from_string("1,3/4 0,3/4");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold1;

    s.fold(fold1);

    s.to_png(string(__func__) + "_post1");
    logging::info() << "Result:\n" << s;
    //   +---------+
    //   |         |
    //   +---------+
    //   |         |
    //   +---------+
    //   |         |
    // 0 +---------+

    auto fold2 = line_segment_from_string("1,1/4 0,1/4");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold2;

    s.fold(fold2);
    s.to_png(string(__func__) + "_post2");
    logging::info() << "Result:\n" << s;
    //   +---------+
    //   |         |
    // 0 +---------+
    //   |         |
    //   +---------+

    // on 0 there is now a line on which we want to fold

    auto fold3 = line_segment_from_string("0,0 1,0");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold3;

    s.fold(fold3);
    s.to_png(string(__func__) + "_post3");
    logging::info() << "Result:\n" << s;
    // 0 +---------+
    //   |         |
    //   +---------+

    // TODO check
}

void testcross()
{
    // cerr << __func__ << endl;

    //   +----+----+
    //   |    |    |
    //   +----+----+
    //   |    |    |
    // 0 +----+----+

    solution s = solution::unit_square();

    auto fold1 = line_segment_from_string("1,1/2 0,1/2");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold1;

    s.fold(fold1);

    s.to_png(string(__func__) + "_post1");
    logging::info() << "Result:\n" << s;

    //   +---------+
    //   |         |
    // 0 +---------+

    auto fold2 = line_segment_from_string("1/2,0 1/2,1");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold2;

    s.fold(fold2);
    s.to_png(string(__func__) + "_post2");
    logging::info() << "Result:\n" << s;
    //   +----+
    //   |    |
    // 0 +----+

    assert(s.source_positions_.size() == 9);

    auto fold3 = line_segment_from_string("1,2 0,0");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold3;

    s.fold(fold3);
    s.to_png(string(__func__) + "_post3");
    logging::info() << "Result:\n" << s;

    auto fold4 = line_segment_from_string("1,1 0,0");
    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold4;

    s.fold(fold4);
    s.to_png(string(__func__) + "_post4");
    logging::info() << "Result:\n" << s;
}

void test1c()
{
    // cerr << __func__ << endl;

    // +---+
    // |   |
    // |   |
    // |   |
    // +---+
    logging::info() << " TEST ONE CORNER !!!!!!!!!! ";
    solution s = solution::unit_square();

    line_segment fold = line_segment_from_string("1/2,0 1,1");

    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold;

    s.fold(fold);
    s.to_png(string(__func__) + "_post");
    logging::info() << "Result:\n" << s;

    // TODO check

    // assert(s.facets_.size() == 2);
}

void test1c2()
{
    logging::info() << " TEST ONE CORNER !!!!!!!!!! ";
    solution s = solution::unit_square();

    line_segment fold = line_segment_from_string("1/2,0 1,1");

    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold;

    s.fold(fold);
    s.to_png(string(__func__) + "_post_1");
    logging::info() << "Result:\n" << s;

    line_segment fold2 = line_segment_from_string("1,1/2 0,1/2");

    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold2;

    s.fold(fold2);
    s.to_png(string(__func__) + "_post_2");
    logging::info() << "Result:\n" << s;

    // TODO check

    // assert(s.facets_.size() == 2);
}

void test2c()
{
    // cerr << __func__ << endl;

    // +---+
    // |   |
    // |   |
    // |   |
    // +---+

    solution s = solution::unit_square();

    line_segment fold = line_segment_from_string("0,0 1,1");

    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold;

    s.fold(fold);
    s.to_png(string(__func__) + "_post");
    logging::info() << "Result:\n" << s;

    //   +-+
    //   | |
    //   | |
    //   | |
    //   +-+

    // TODO check

    // assert(s.facets_.size() == 2);
}

void testedge()
{
    // cerr << __func__ << endl;

    // +---+
    // |   |
    // |   |
    // |   |
    // +---+

    solution s = solution::unit_square();

    line_segment fold = line_segment_from_string("0,0 1,0");

    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold;

    s.fold(fold);
    s.to_png(string(__func__) + "_post");
    logging::info() << "Result:\n" << s;

    //   +-+
    //   | |
    //   | |
    //   | |
    //   +-+

    // TODO check

    // assert(s.facets_.size() == 2);
}
