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

    stringstream s;
    s << p;
    assert(s.str() == "1/2,3/4");

    point q(p);
    point r = p;

    assert(q == p);
    assert(r == p);
}
