
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
    skeleton(const std::vector<line_segment>& es) : edges(es)
    {
        // FIXME filter unique lines
        //        std::set<line> unique_lines;
        for (const auto& edge : es)
        {
            //            unique_lines.emplace(edge.source(), edge.target());
            //            unique_lines.emplace(edge.target(), edge.source());
            lines_.emplace_back(edge.source(), edge.target());
            lines_.emplace_back(edge.target(), edge.source());
        }
        //        lines_ = std::vector<line>(unique_lines.begin(), unique_lines.end());
    }

    const std::vector<line>& lines() const
    {
        return lines_;
    }

    std::vector<line_segment> edges;

private:
    std::vector<line> lines_;
};

}

#endif // SOLVER_SKELETON_HPP
