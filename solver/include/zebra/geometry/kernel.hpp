#ifndef SOLVER_GEOMETRY_KERNEL
#define SOLVER_GEOMETRY_KERNEL

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Gmpz.h>

namespace zebra
{
using kernel = CGAL::Simple_cartesian<CGAL::Gmpq>;
}

#endif
