
#ifndef SOLVER_POLYGON_HPP
#define SOLVER_POLYGON_HPP

#include "kernel.hpp"

#include <CGAL/Polygon_2.h>

#include <vector>

namespace zebra
{

class polygon : public CGAL::Polygon_2<kernel>
{
public:
    template <class CONTAINER>
    polygon(const CONTAINER& c) : CGAL::Polygon_2<kernel>(c.begin(), c.end())
    {
    }
};
}

#endif // SOLVER_POLYGON_HPP
