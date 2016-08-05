
#ifndef SOLVER_SILHOUETTE_HPP
#define SOLVER_SILHOUETTE_HPP

#include <vector>

namespace zebra
{
class silhouette
{
public:
    silhouette(std::vector<polygon> p) : polygons(p)
    {
    }
    std::vector<polygon> polygons;
};
}

#endif // SOLVER_SILHOUETTE_HPP
