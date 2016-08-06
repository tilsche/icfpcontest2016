
#ifndef SOLVER_SOLUTION_HPP
#define SOLVER_SOLUTION_HPP

#include "geometry.hpp"

#include <zebra/log.hpp>

#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

namespace zebra
{

struct solution
{
    std::vector<point> source_positions;
    std::vector<std::vector<int>> facets;
    std::vector<point> destination_positions;

    std::vector<polygon> source_facets() const
    {
        std::vector<polygon> sf(facets.size());
        size_t i = 0;
        for (const auto& facet : facets)
        {
            for (size_t vid = 0; vid < facet.size(); vid++)
            {
                sf[i].push_back(source_positions[vid]);
            }
            i++;
        }
        return sf;
    }

    std::vector<polygon> destination_facets() const
    {
        std::vector<polygon> sf(facets.size());
        size_t i = 0;
        for (const auto& facet : facets)
        {
            for (size_t vid = 0; vid < facet.size(); vid++)
            {
                sf[i].push_back(destination_positions[vid]);
            }
            i++;
        }
        return sf;
    }

    bool is_valid() const
    {
        // All source vertices in 0,0 1,1
        // No duplicate source vertex
        std::set<point> source_set;
        for (const auto& v : source_positions)
        {
            if (v.x() > 1 || v.x() < 0 || v.y() > 1 || v.y() < 0)
            {
                return false;
            }

            auto ret = source_set.insert(v);
            if (!ret.second)
            {
                return false;
            }
        }

        assert(source_positions.size() == destination_positions.size());

        auto sfs = source_facets();
        auto dfs = destination_facets();

        assert(dfs.size() == sfs.size());

        for (size_t i = 0; i < dfs.size(); i++)
        {
            auto& sf = sfs[i];
            auto& df = dfs[i];
            assert(sf.size() == sf.size());

            if (!sf.is_simple() || !df.is_simple())
            {
                return false;
            }

            for (size_t ei = 0; ei < sf.size(); ei++)
            {
                // TODO Check possible transformation!!!!
                auto source_edge = sf.edge(ei);
                auto destination_edge = df.edge(ei);

                if (source_edge.squared_length() == 0)
                {
                    return false;
                }
                if (source_edge.squared_length() != destination_edge.squared_length())
                {
                    return false;
                }
            }

            return true;
        }
    }


    void fold(const line_segment& fold) {

        auto verify_fold = [this](const line_segment& fold) {
            int intersections = 0;
            int is_ons        = 0;
            int on_segments   = 0;
            std::vector<line_segment> segments;
            for (auto facet : facets) {
                for (int i = 1; i < facet.size(); i += 1) {
                    segments.push_back(line_segment(destination_positions[i-1], destination_positions[i]));
                }
                segments.push_back(line_segment(destination_positions[facet.size()-1], destination_positions[0]));
            }

            for (const auto& s : segments) {
                boost::optional<boost::variant<point, line_segment>> o = CGAL::intersection(s, fold);

                if (o == boost::none) { continue; }

                if (o->which() == 0) {
                    intersections += 1;
                    assert(! (s.has_on(fold.source()) && s.has_on(fold.target())));
                    if (s.has_on(fold.source()) || s.has_on(fold.target())) {
                        is_ons += 1;
                    }
                } else {
                    on_segments += 1;
                }
            }

            if (is_ons == 0) {
                logging::error() << "Fold does not start and end on any line segment";
                assert(is_ons > 0);
            }

            if (intersections % 2 != 0) {
                logging::warn() << "Fold intersects with an odd number of segments: " << intersections;
            }

            if (on_segments > 0) {
                logging::info() << "Fold overlaps with segments: " << on_segments;
            }

            if (is_ons % 2 != 0) {
                logging::warn() << "Fold ends/starts on an odd number of segments: " << is_ons;
            }
        };

        verify_fold(fold);


        // intersection points

        for (const auto& facet : facets) {
            std::vector<line_segment> segments;
            for (int i = 1; i < facet.size(); i += 1) {
                segments.push_back(line_segment(destination_positions[i-1], destination_positions[i]));
            }
            segments.push_back(line_segment(destination_positions[destination_positions.size()-1], destination_positions[0]));

            std::vector<std::pair<point, const line_segment*>> intersection_points;

            for (const auto& l : segments) {
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

            facets.resize(0);
            if (intersection_points.size() > 2) {
                std::cerr << "Unexpected: intersection point number > 2" << std::endl;
            } else if (intersection_points.size() == 1) {
                std::cerr << "Unexpected: intersection point number == 1" << std::endl;
            } else if (intersection_points.size() == 0) {
                facets.push_back(facet);
                continue;
            }

            auto i1 = source_positions.size();
            auto i2 = source_positions.size() + 1;

            source_positions.push_back(intersection_points[0].first);
            source_positions.push_back(intersection_points[1].first);
            destination_positions.push_back(intersection_points[0].first);
            destination_positions.push_back(intersection_points[1].first);

            facets.push_back(std::vector<int>{facet[0]});
            facets.push_back(std::vector<int>{});
        }

        // // cut

        // std::vector<line_segment> t;

        // for (std::size_t i = 0; i < s.edges.size(); i += 1) {
        //     bool found = false;
        //     point q;
        //     for (const auto& p : intersection_points) {
        //         if (p.second == &s.edges[i]) {
        //             q = p.first;
        //             found = true;
        //             break;
        //         }
        //     }

        //     if (found == false) {
        //         t.push_back(s.edges[i]);
        //     } else {
        //         t.push_back({s.edges[i].source(), q});
        //         t.push_back({q,                   s.edges[i].target()});
        //     }
        // }

        // t.push_back(fold);

        // std::cerr << "split segments: " << std::endl;
        // for (const auto& l : t) {
        //     std::cerr << "  " << line_segment_to_string(l) << std::endl;
        // }

        // // determine which half to fold

        // std::vector<line_segment> v1;
        // std::vector<line_segment> v2;

        // auto left_side = [](const line_segment& fold, const line_segment& l) -> bool {

        //     auto distance = [](const line_segment& fold, const point& p) -> CGAL::Gmpq {
        //         auto x1 = fold.source().x();
        //         auto y1 = fold.source().y();
        //         auto x2 = fold.target().x();
        //         auto y2 = fold.target().y();
        //         auto x  = p.x();
        //         auto y  = p.y();

        //         return (x - x1)*(y2 - y1) - (y - y1)*(x2 - x1);
        //     };

        //     if (distance(fold, l.source()) <= 0 && distance(fold, l.target()) <= 0) {
        //         return true;
        //     } else {
        //         return false;
        //     }
        // };

        // for (const auto& l : t) {
        //     if (left_side(fold, l)) {
        //         v1.push_back(l);
        //     } else {
        //         v2.push_back(l);
        //     }
        // }

        // std::cerr << "left segments: " << std::endl;
        // for (const auto& l : v1) {
        //     std::cerr << "  " << line_segment_to_string(l) << std::endl;
        // }

        // std::cerr << "right segments: " << std::endl;
        // for (const auto& l : v2) {
        //     std::cerr << "  " << line_segment_to_string(l) << std::endl;
        // }

        // // TODO mirror left side

        // // TODO unify mirrored left side and right side

        // // TODO deduplicate segments
    }
};

inline std::ostream& operator<<(std::ostream& os, const solution& s)
{
    assert(s.source_positions.size() == s.destination_positions.size());
    os << s.source_positions.size() << "\n";
    for (const auto& vertex : s.source_positions)
    {
        os << point_to_string(vertex) << "\n";
    }

    os << s.facets.size() << "\n";
    for (const auto& facet : s.facets)
    {
        os << facet.size() << " ";
        for (auto vertex_id : facet)
        {
            os << vertex_id << " ";
        }
        os << "\n";
    }

    for (const auto& vertex : s.destination_positions)
    {
        os << point_to_string(vertex) << "\n";
    }
    return os;
}


}

#endif // SOLVER_SOLUTION_HPP
