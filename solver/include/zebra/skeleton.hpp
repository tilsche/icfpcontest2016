
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

    //auto left_side = [](const line_segment& fold, const line_segment& l) -> bool {
    //    auto left_side(const line_segment& fold, const point& p) -> bool {
    //    }


    //    if (left_side(fold, l.source())

    //};

    for (const auto& l : t) {

        std::cerr << CGAL::squared_distance(fold, l.source());

    }



    return skeleton();

}

}

#endif // SOLVER_SKELETON_HPP
