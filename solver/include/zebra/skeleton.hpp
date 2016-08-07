
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

    skeleton(const std::vector<line_segment>& line_segments) : edges(line_segments)
    {
        for (const auto& s : line_segments)
        {
            CGAL::Line_2<kernel> l(s);

            bool found = false;
            for (const auto& u : unique_lines_) {
                if (l == u) {
                    found = true;
                    break;
                }
            }

            if (found == false) {
                unique_lines_.emplace_back(l);
            }

            lines_.emplace_back(l);
        }
    }

    const std::vector<line>& lines() const
    {
        return lines_;
    }

    const std::vector<line>& unique_lines() const
    {
        return unique_lines_;
    }

    std::vector<line_segment> edges;

private:
    std::vector<line> lines_;
    std::vector<line> unique_lines_;
};
}

#endif // SOLVER_SKELETON_HPP
