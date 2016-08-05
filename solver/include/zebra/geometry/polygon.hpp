
#ifndef SOLVER_POLYGON_HPP
#define SOLVER_POLYGON_HPP

#include "kernel.hpp"

#include <vector>

namespace zebra
{

class polygon : kernel::Polygon_2
{
public:
    template <class CONTAINER>
    polygon(const CONTAINER& c) : kernel::Polygon_2(c.begin(), c.end())
    {
    }
};
}

#endif // SOLVER_POLYGON_HPP
