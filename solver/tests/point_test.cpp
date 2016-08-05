#include <zebra/point.hpp>
using namespace zebra;

#include <sstream>
using namespace std;

void test1();

int main() {
    test1();
}

void test1() {
    point p = point::from_string("1/2,3/4");
    assert(p.x.get_num() == 1);
    assert(p.x.get_den() == 2);
    assert(p.y.get_num() == 3);
    assert(p.y.get_den() == 4);

    stringstream s1;
    s1 << p;
    assert(s1.str() == "1/2,3/4");

    point q(p);
    point r = p;

    assert(q == p);
    assert(r == p);

    p = point::from_string("1,2");
    assert(p.x.get_num() == 1);
    assert(p.x.get_den() == 1);
    assert(p.y.get_num() == 2);
    assert(p.y.get_den() == 1);

    stringstream s2;
    s2 << p;
    assert(s2.str() == "1,2");
}
