#include <zebra/geometry/line_segment.hpp>
using namespace zebra;

#include <sstream>
using namespace std;

void test1();
void test_slope();
void test_disjoint();

int main() {
    test1();
    test_slope();
    test_disjoint();
}

void test1() {
    line_segment l = line_segment::from_string("1/2,3/4 5/6,7/8");
    assert(l.a.x.get_num() == 1);
    assert(l.a.x.get_den() == 2);
    assert(l.a.y.get_num() == 3);
    assert(l.a.y.get_den() == 4);
    assert(l.b.x.get_num() == 5);
    assert(l.b.x.get_den() == 6);
    assert(l.b.y.get_num() == 7);
    assert(l.b.y.get_den() == 8);

    stringstream s;
    s << l;
    assert(s.str() == "1/2,3/4 5/6,7/8");

    line_segment m(l);
    line_segment n = l;

    assert(m == l);
    assert(n == l);
}

void test_slope() {
    line_segment l1 = line_segment::from_string("1,1 2,2");
    line_segment l2 = line_segment::from_string("2,2 1,1");
    line_segment l3 = line_segment::from_string("0,0 0,1");
    line_segment l4 = line_segment::from_string("0,0 0,-1");

    assert(l1.slope() == 1);
    assert(l2.slope() == 1);
    assert(l3.slope_inf() == true);
    assert(l4.slope_inf() == true);
}

void test_disjoint() {
    line_segment l1 = line_segment::from_string("1,1 2,2");
    line_segment l2 = line_segment::from_string("2,2 3,3");
    line_segment l3 = line_segment::from_string("3,3 4,4");
    line_segment l4 = line_segment::from_string("0,0 0,1");

    line_segment l5 = line_segment::from_string("3/2,3/2 7/4,7/4");
    line_segment l6 = line_segment::from_string("3/2,3/2 3,3");

    assert(is_disjoint(l1, l2));
    assert(is_disjoint(l2, l3));
    assert(is_disjoint(l1, l3));
    assert(is_disjoint(l1, l4));

    assert(is_disjoint(l1, l5) == false);
    assert(is_disjoint(l1, l6) == false);

}
