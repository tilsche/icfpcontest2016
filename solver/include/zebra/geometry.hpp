#ifndef SOLVER_GEOMETRY_HPP
#define SOLVER_GEOMETRY_HPP

#include "geometry/kernel.hpp"
#include "geometry/line_segment.hpp"
#include "geometry/point.hpp"
#include "geometry/polygon.hpp"

#include <CGAL/Aff_transformation_2.h>
//#include <CGAL/Homogeneous/Aff_transformationH2.h>

namespace zebra
{
using transformation = CGAL::Aff_transformation_2<kernel>;
// using transformation = CGAL::Aff_transformationH2<kernel>;
using vector = kernel::Vector_2;
using line = kernel::Line_2;

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
}

#endif // SOLVER_GEOMETRY_HPP
