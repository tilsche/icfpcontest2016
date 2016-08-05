
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


skeleton skeleton_fold(const skeleton& s, const line_segment& fold) {

    // TODO test that fold is a correct line .. has_on()

    // intersection points
    std::vector<std::pair<point, const line_segment*>> intersection_points;

    for (const auto& l : s.edges) {
        boost::optional<boost::variant<point, line_segment>> o = CGAL::intersection(l, fold);

        if (o == boost::none) { continue; }

        if (o->which() == 0) {
            intersection_points.push_back(std::make_pair(boost::get<point>(*o), &l));
        } else {
            std::cerr << "folding on a line segment, fold:" << line_segment_to_string(fold) << ", segment:" << line_segment_to_string(l) << std::endl;
        }
    }

    std::cerr << "intersection points: " << std::endl;
    for (const auto& p : intersection_points) {
        std::cerr << "  p:" << point_to_string(p.first) << ", segment:" << line_segment_to_string(*p.second) << std::endl;
    }

    // cut

    std::vector<line_segment> t;

    for (std::size_t i = 0; i < s.edges.size(); i += 1) {
        bool found = false;
        point q;
        for (const auto& p : intersection_points) {
            if (p.second == &s.edges[i]) {
                q = p.first;
                found = true;
                break;
            }
        }

        if (found == false) {
            t.push_back(s.edges[i]);
        } else {
            t.push_back({s.edges[i].source(), q});
            t.push_back({q,                   s.edges[i].target()});
        }
    }

    t.push_back(fold);

    std::cerr << "split segments: " << std::endl;
    for (const auto& l : t) {
        std::cerr << "  " << line_segment_to_string(l) << std::endl;
    }

    // determine which half to fold

    std::vector<line_segment> v1;
    std::vector<line_segment> v2;

    auto left_side = [](const line_segment& fold, const line_segment& l) -> bool {

        auto distance = [](const line_segment& fold, const point& p) -> CGAL::Gmpq {
            auto x1 = fold.source().x();
            auto y1 = fold.source().y();
            auto x2 = fold.target().x();
            auto y2 = fold.target().y();
            auto x  = p.x();
            auto y  = p.y();

            return (x - x1)*(y2 - y1) - (y - y1)*(x2 - x1);
        };

        if (distance(fold, l.source()) <= 0 && distance(fold, l.target()) <= 0) {
            return true;
        } else {
            return false;
        }
    };

    for (const auto& l : t) {
        if (left_side(fold, l)) {
            v1.push_back(l);
        } else {
            v2.push_back(l);
        }
    }

    std::cerr << "left segments: " << std::endl;
    for (const auto& l : v1) {
        std::cerr << "  " << line_segment_to_string(l) << std::endl;
    }

    std::cerr << "right segments: " << std::endl;
    for (const auto& l : v2) {
        std::cerr << "  " << line_segment_to_string(l) << std::endl;
    }

    // TODO mirror left side

    // TODO unify mirrored left side and right side

    // TODO deduplicate segments

    return skeleton();
}

}

#endif // SOLVER_SKELETON_HPP
