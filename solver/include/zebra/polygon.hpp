
#ifndef SOLVER_POLYGON_HPP
#define SOLVER_POLYGON_HPP

#include "point.hpp"

#include <vector>

namespace zebra
{

class polygon
{
public:
    polygon(std::vector<point> ps) : points(ps)
    {
    }

    std::vector<point> points;
};
}

#endif // SOLVER_POLYGON_HPP
