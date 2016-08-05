#include <zebra/skeleton.hpp>
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

    skeleton s;
    s.edges = {{{0, 0}, {1, 0}}, 
               {{1, 0}, {1, 1}},
               {{1, 1}, {0, 1}},
               {{0, 1}, {0, 0}}};

    line_segment fold = line_segment_from_string("0,1/2 1,3/4");

    auto o = skeleton_fold(s, fold);
    // TODO
}

void test2() {

    skeleton s;
    s.edges = {{{0, 0}, {1, 0}},
               {{1, 0}, {1, 1}},
               {{1, 1}, {0, 1}},
               {{0, 1}, {0, 0}},
               {{0, 0}, {1, 1}},
               {{0, 1}, {1, 0}},
    };

    line_segment fold = line_segment_from_string("0,1/2 1,3/4");

    auto o = skeleton_fold(s, fold);
    // TODO
}
