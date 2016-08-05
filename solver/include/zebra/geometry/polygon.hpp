
#ifndef SOLVER_POLYGON_HPP
#define SOLVER_POLYGON_HPP

#include "kernel.hpp"

#include <CGAL/Polygon_2.h>

namespace zebra
{
using polygon = CGAL::Polygon_2<kernel>;
}
#endif // SOLVER_POLYGON_HPP