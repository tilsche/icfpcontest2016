#ifndef SOLVER_CGAL_HPP
#define SOLVER_CGAL_HPP

#include <CGAL/Gmpz.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Aff_transformation_2.h>
#include <CGAL/Polygon_set_2.h>

namespace zebra
{
using kernel = CGAL::Simple_cartesian<CGAL::Gmpq>;

using point = kernel::Point_2;
using vector = kernel::Vector_2;
using line = kernel::Line_2;
using line_segment = kernel::Segment_2;

using polygon = CGAL::Polygon_2<kernel>;
using polygon_with_holes = CGAL::Polygon_with_holes_2<kernel>;
using transformation = CGAL::Aff_transformation_2<kernel>;
using polygon_set = CGAL::Polygon_set_2<kernel>;
}

#endif // SOLVER_CGAL_HPP
