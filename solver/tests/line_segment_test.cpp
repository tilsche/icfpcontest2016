#include <zebra/geometry/line_segment.hpp>
using namespace zebra;

#include <sstream>
using namespace std;

void test1();
void test_disjoint();

int main() {
    test1();
    test_disjoint();
}

void test1() {
    line_segment l = line_segment_from_string("1/2,3/4 5/6,7/8");
    assert(l.source().x().numerator()   == 1);
    assert(l.source().x().denominator() == 2);
    assert(l.source().y().numerator()   == 3);
    assert(l.source().y().denominator() == 4);
    assert(l.target().x().numerator()   == 5);
    assert(l.target().x().denominator() == 6);
    assert(l.target().y().numerator()   == 7);
    assert(l.target().y().denominator() == 8);

    stringstream s;
    s << line_segment_to_string(l);
    assert(s.str() == "1/2,3/4 5/6,7/8");

    line_segment m(l);
    line_segment n = l;

    assert(m == l);
    assert(n == l);
}

void test_disjoint() {
    line_segment l1 = line_segment_from_string("1,1 2,2");
    line_segment l2 = line_segment_from_string("2,2 3,3");
    line_segment l3 = line_segment_from_string("3,3 4,4");
    line_segment l4 = line_segment_from_string("0,0 0,1");

    line_segment l5 = line_segment_from_string("3/2,3/2 7/4,7/4");
    line_segment l6 = line_segment_from_string("3/2,3/2 3,3");

    //std::cerr << l1 << "," << l2 << std::endl;
    assert(is_disjoint(l1, l2));
    assert(is_disjoint(l2, l3));
    assert(is_disjoint(l1, l3));
    assert(is_disjoint(l1, l4));

    assert(is_disjoint(l1, l5) == false);
    assert(is_disjoint(l1, l6) == false);

}
