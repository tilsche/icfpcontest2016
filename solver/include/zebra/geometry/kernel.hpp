#ifndef SOLVER_GEOMETRY_KERNEL
#define SOLVER_GEOMETRY_KERNEL

#include <CGAL/Gmpz.h>
#include <CGAL/Simple_cartesian.h>

namespace zebra
{
using kernel = CGAL::Simple_cartesian<CGAL::Gmpq>;

inline double gmpq_to_double(CGAL::Gmpq q)
{
    return q.numerator().to_double() / q.denominator().to_double();
}
}

#endif
