
#ifndef SOLVER_POLYGON_HPP
#define SOLVER_POLYGON_HPP

#include "kernel.hpp"

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

namespace zebra
{
using polygon = CGAL::Polygon_2<kernel>;
using polygon_with_holes = CGAL::Polygon_with_holes_2<kernel>;

inline polygon make_polygon_1()
{
    polygon p;
    p.push_back({0, 0});
    p.push_back({1, 0});
    p.push_back({1, 1});
    p.push_back({0, 1});
    return p;
}
}

#endif // SOLVER_POLYGON_HPP
