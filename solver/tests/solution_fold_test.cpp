#include <zebra/solution.hpp>
using namespace std;
using namespace zebra;

void test1();
void test2();
void test3();
void test4();

int main() {
    test1();
    test2();
    test3();
    test4();

    return 0;
}

void test1() {
    //cerr << __func__ << endl;

    // +---+
    // |   |
    // |   |
    // |   |
    // +---+

    solution s;
    s.source_positions      = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    s.facets                = {{0, 1, 2, 3}};
    s.destination_positions = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

    line_segment fold = line_segment_from_string("1/2,0 1/2,1");

    s.fold(fold);

    //   +-+
    //   | |
    //   | |
    //   | |
    //   +-+

    // TODO check

    //assert(s.facets.size() == 2);
}

void test2() {
    //cerr << __func__ << endl;

    // +---+
    // |   |
    // |   |
    // |   |
    // +---+

    solution s;
    s.source_positions      = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    s.facets                = {{0, 1, 2, 3}};
    s.destination_positions = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

    s.fold(line_segment_from_string("0,1/2 1,3/4"));
    //
    //
    // +---+
    // |   |
    // +---+

    // TODO check
    //assert(s.facets.size() == 2);
}

void test3() {
    //cerr << __func__ << endl;

    // +---------+
    // |         |
    // +---------+
    // |         |
    // +---------+

    solution s;
    s.source_positions = {{0, 0  }, {1, 0  }, {1, 1}, {0, 1},
                          {0, 1/2}, {1, 1/2}};

    s.facets = { {0, 1, 4, 5}, {5, 4, 2, 3}};

    //
    //
    // +---------+
    // |         |
    // +---------+
    s.destination_positions = {{0,   0}, {1,   0}, {1, 0}, {0, 0},
                               {0, 1/2}, {1, 1/2}};

    s.fold(line_segment_from_string("1/2,0 1/2,1"));
    //
    //
    //    +--+
    //    |  |
    //    +--+

    // TODO check
    //assert(s.facets.size() == 4);
}

void test4() {
    //cerr << __func__ << endl;

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
    s.source_positions = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    s.facets = {{0, 1, 2, 3}};
    s.destination_positions = {{0,   0}, {1,   0}, {1, 0}, {0, 0}};

    s.fold(line_segment_from_string("0,3/4 1/3/4"));
    //   +---------+
    //   |         |
    //   +---------+
    //   |         |
    //   +---------+
    //   |         |
    // 0 +---------+

    s.fold(line_segment_from_string("0,1/4 1/1/4"));
    //   +---------+
    //   |         |
    // 0 +---------+
    //   |         |
    //   +---------+

    // on 0 there is now a line on which we want to fold

    s.fold(line_segment_from_string("0,0 1,0"));
    // 0 +---------+
    //   |         |
    //   +---------+

    // TODO check
}
