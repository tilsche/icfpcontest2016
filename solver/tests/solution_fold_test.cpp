#include <zebra/solution.hpp>
using namespace std;
using namespace zebra;

void test1();
void test2();
void test3();
void test4();
void test1c();
void test2c();
void testedge();

int main()
{
    test1();
    test2();
    test3();
    test4();

    test1c();
    test2c();
    testedge();
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

    solution s;
    s.source_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
    s.facets = { facet{.vertex_ids = { 0, 1, 2, 3 } } };
    s.destination_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };


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

    // assert(s.facets.size() == 2);
}

void test2()
{
    // cerr << __func__ << endl;

    // +---+
    // |   |
    // |   |
    // |   |
    // +---+

    solution s;
    s.source_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
    s.facets = { facet{.vertex_ids = { 0, 1, 2, 3 } } };
    s.destination_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };


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
    // assert(s.facets.size() == 2);
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
    s.source_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { 1, CGAL::Gmpq(1,2) }, { 0, CGAL::Gmpq(1,2) } };

    s.facets = { facet{.vertex_ids = { 0, 1, 4, 5 } }, facet{.vertex_ids = { 5, 4, 2, 3 } } };

    //
    //
    // +---------+
    // |         |
    // +---------+
    s.destination_positions = {
        { 0, 0 }, { 1, 0 }, { 1, 0 }, { 0, 0 }, { 1, CGAL::Gmpq(1,2) }, { 0, CGAL::Gmpq(1,2) }
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
    // assert(s.facets.size() == 4);
}

void test4()
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

    solution s;
    s.source_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
    s.facets = { facet{.vertex_ids = { 0, 1, 2, 3 } } };
    s.destination_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };

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

void test1c()
{
    // cerr << __func__ << endl;

    // +---+
    // |   |
    // |   |
    // |   |
    // +---+
    logging::info() << " TEST ONE CORNER !!!!!!!!!! ";
    solution s;
    s.source_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
    s.facets = { facet{.vertex_ids = { 0, 1, 2, 3 } } };
    s.destination_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };

    line_segment fold = line_segment_from_string("1/2,0 1,1");

    logging::info() << "Testing to fold:\n" << s;
    logging::info() << "with line segment " << fold;

    s.fold(fold);
    s.to_png(string(__func__) + "_post");
    logging::info() << "Result:\n" << s;

    // TODO check

    // assert(s.facets.size() == 2);
}

void test2c()
{
    // cerr << __func__ << endl;

    // +---+
    // |   |
    // |   |
    // |   |
    // +---+

    solution s;
    s.source_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
    s.facets = { facet{.vertex_ids = { 0, 1, 2, 3 } } };
    s.destination_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };

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

    // assert(s.facets.size() == 2);
}

void testedge()
{
    // cerr << __func__ << endl;

    // +---+
    // |   |
    // |   |
    // |   |
    // +---+

    solution s;
    s.source_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
    s.facets = { facet{.vertex_ids = { 0, 1, 2, 3 } } };
    s.destination_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };

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

    // assert(s.facets.size() == 2);
}
