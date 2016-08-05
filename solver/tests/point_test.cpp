#include <zebra/geometry/point.hpp>

#include <sstream>

void test1();

int main()
{
    test1();
}

void test1()
{
    zebra::point p = zebra::point_from_string("1/2,3/4");
    assert(p.x().numerator() == 1);
    assert(p.x().denominator() == 2);
    assert(p.y().numerator() == 3);
    assert(p.y().denominator() == 4);

    //std::cerr << zebra::point_to_string(p);
    assert(zebra::point_to_string(p) == "1/2,3/4");

    zebra::point q(p);
    zebra::point r = p;

    assert(q == p);
    assert(r == p);

    p = zebra::point_from_string("1,2");
    assert(p.x().numerator()   == 1);
    assert(p.x().denominator() == 1);
    assert(p.y().numerator()   == 2);
    assert(p.y().denominator() == 1);

    //std::cerr <<zebra::point_to_string(p);
    assert(zebra::point_to_string(p) == "1,2");
}
