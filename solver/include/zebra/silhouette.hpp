
#ifndef SOLVER_SILHOUETTE_HPP
#define SOLVER_SILHOUETTE_HPP

#include <vector>

#include <zebra/geometry/polygon.hpp>

#include <CGAL/Boolean_set_operations_2.h>

namespace zebra
{
class silhouette
{
public:
    silhouette(std::vector<polygon> polys) : polygons_(polys)
    {
        std::vector<polygon> positive_polys;
        std::vector<polygon> holes;
        for (const auto& p : polys)
        {
            if (p.is_counterclockwise_oriented())
            {
                positive_polys.push_back(p);
            }
            else
            {
                holes.push_back(p);
            }
        }

        std::vector<polygon_with_holes> union_polys;
        CGAL::join(positive_polys.begin(), positive_polys.end(), std::back_inserter(union_polys));
        assert(positive_polys.size() == 1);

        auto& union_poly = union_polys[0];

        for (const auto& h : holes)
        {
            union_poly.add_hole(h);
        }

        shape_ = union_poly;

        //        holes.insert(union_poly.holes_begin(), union_poly.holes_end());
        //        polygon_with_holes final_shape(union_poly, holes.begin(), holes.end()) shape_ =
        // final_shape;
    }

    const std::vector<polygon>& polygons() const
    {
        return polygons_;
    }

    const polygon_with_holes& shape() const
    {
        return shape_;
    }

private:
    std::vector<polygon> polygons_;
    polygon_with_holes shape_;
};
}

#endif // SOLVER_SILHOUETTE_HPP
