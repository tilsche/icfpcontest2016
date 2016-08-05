#ifndef SOLVER_GEOMETRY_HPP
#define SOLVER_GEOMETRY_HPP

#include "geometry/kernel.hpp"
#include "geometry/line_segment.hpp"
#include "geometry/point.hpp"
#include "geometry/polygon.hpp"

#include <CGAL/Aff_transformation_2.h>

namespace zebra
{
using transofrmation = CGAL::Aff_transformation_2<kernel>;
using vector = kernel::Vector_2;
}

#endif // SOLVER_GEOMETRY_HPP
