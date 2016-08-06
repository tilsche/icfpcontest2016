#include <zebra/solution.hpp>
using namespace std;
using namespace zebra;

void test1();
void test2();

int main() {
    test1();
    test2();

    return 0;
}

void test1() {

    solution s;
    s.source_positions      = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    s.facets                = {{0, 1, 2, 3}};
    s.destination_positions = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

    line_segment fold = line_segment_from_string("0,1/2 1,3/4");

    s.fold(fold);
    // TODO
}

void test2() {

    // +---------+
    // |         |
    // +---------+
    // |         |
    // +---------+

    solution s;
    s.source_positions = {{0, 0}, {1, 0}, {1, 1}, {0, 1},
                          {0, 1/2}, {1, 1/2}};

    s.facets = { {0, 1, 4, 5}, {4, 5, 2, 3}};

    s.destination_positions = {{0, 0}, {1, 0}, {1, 0}, {0, 0},
                               {0, 1/2}, {1, 1/2}};

    line_segment fold = line_segment_from_string("1/2,0 1/2,1");

    s.fold(fold);
    // TODO
}
