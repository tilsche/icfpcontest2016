#ifndef SOLVER_GEOMETRY_HPP
#define SOLVER_GEOMETRY_HPP

#include "geometry/cgal.hpp"
#include "geometry/gmp.hpp"
#include "geometry/line_segment.hpp"
#include "geometry/point.hpp"
#include "geometry/polygon.hpp"

#include <zebra/log.hpp>

namespace zebra
{
transformation reflection(line l)
{
    const auto& a = l.a();
    const auto& b = l.b();
    const auto& c = l.c();
    // IT's MAGIC!
    auto de = a * a + b * b;
    auto aa = b * b - a * a;
    auto bb = a * a - b * b;
    auto ab = a * b * -2;
    auto ac = a * c * -2;
    auto bc = b * c * -2;
    return transformation(aa, ab, ac, ab, bb, bc, de);
}

transformation rotation(const line_segment& segment)
{
    auto& source = segment.source();
    auto& target = segment.target();

    auto cos_rho = target.x() - source.x();
    auto sin_rho = target.y() - source.y();

    auto hw2 = segment.squared_length();

    CGAL::Gmpq hw;

    if (CGAL::is_square(hw2, hw))
    {
        return transformation(CGAL::ROTATION, sin_rho, cos_rho, hw);
    }

    logging::debug() << "Requested rotation cannot be performed: " << segment;

    return transformation();
}

transformation translation(const vector& d)
{
    return transformation(CGAL::TRANSLATION, d);
}

transformation translation(const point& p)
{
    return translation(vector(p.x(), p.y()));
}

CGAL::Gmpq holy_area(const polygon_with_holes& pholy)
{
    auto a = pholy.outer_boundary().area();
    for (auto holeit = pholy.holes_begin(); holeit != pholy.holes_end(); holeit++)
    {
        a -= holeit->area();
    }
    return a;
}
}

#endif // SOLVER_GEOMETRY_HPP
