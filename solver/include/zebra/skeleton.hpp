
#ifndef SOLVER_SKELETON_HPP
#define SOLVER_SKELETON_HPP

#include "geometry/line_segment.hpp"

#include <vector>

namespace zebra
{
class skeleton
{
public:
    skeleton()
    {
    }
    skeleton(const std::vector<line_segment>& e) : edges(e)
    {
    }

    std::vector<line_segment> edges;
};
}

#endif // SOLVER_SKELETON_HPP
