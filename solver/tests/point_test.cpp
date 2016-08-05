#include <zebra/geometry/point.hpp>

#include <sstream>

void test1();

int main()
{
    test1();
}

void test1()
{
    auto p = zebra::point("1/2,3/4");
    assert(p.x().numerator() == 1);
    assert(p.x().denominator() == 2);
    assert(p.y().numerator() == 3);
    assert(p.y().denominator() == 4);

    std::stringstream s;
    s << p;
    std::cout << s.str();
    assert(s.str() == "1/2,3/4");

    zebra::point q(p);
    zebra::point r = p;

    assert(q == p);
    assert(r == p);

    p = zebra::point::from_string("1,2");
    assert(p.x().numerator() == 1);
    assert(p.x().denominator() == 1);
    assert(p.y().denominator() == 2);
    assert(p.y().denominator() == 1);

    std::stringstream s2;
    s2 << p;
    assert(s2.str() == "1,2");
}
