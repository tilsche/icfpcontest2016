
#ifndef SOLVER_SILHOUETTE_HPP
#define SOLVER_SILHOUETTE_HPP

#include <vector>

#include "geometry/polygon.hpp"

namespace zebra
{
class silhouette
{
public:
    silhouette()
    {
    }
    silhouette(std::vector<polygon> p) : polygons(p)
    {
    }
    std::vector<polygon> polygons;
};
}

#endif // SOLVER_SILHOUETTE_HPP
