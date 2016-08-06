
#ifndef SOLVER_SOLUTION_HPP
#define SOLVER_SOLUTION_HPP

#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include <zebra/geometry.hpp>
#include <zebra/log.hpp>

namespace zebra
{

struct facet
{
    size_t size() const
    {
        return vertex_ids.size();
    }

    size_t operator[](size_t index) const
    {
        return vertex_ids[index % size()];
    }

    std::vector<size_t> vertex_ids;
    transformation transform;
};

struct solution
{
    std::vector<point> source_positions;
    std::vector<facet> facets;
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
        }
        return true;
    }

    line_segment destination_segment(const facet& facet, size_t segment_id) const
    {
        assert(segment_id < facet.size());
        return line_segment(destination_positions[facet[segment_id]],
                            destination_positions[facet[segment_id + 1]]);
    }

    line_segment source_segment(const facet& facet, size_t segment_id) const
    {
        assert(segment_id < facet.size());
        return line_segment(source_positions[facet[segment_id]],
                            source_positions[facet[segment_id + 1]]);
    }

    void facet_mirror(facet& facet, line fold_line)
    {
    }

    void facet_fold(facet& facet, line fold_line)
    {
        size_t positive_points = 0;
        size_t negative_points = 0;
        for (auto vertex_id : facet.vertex_ids)
        {
            const auto& vertex = destination_positions[vertex_id];
            switch (fold_line.oriented_side(vertex))
            {
            case CGAL::ON_POSITIVE_SIDE:
                positive_points++;
                break;
            case CGAL::ON_NEGATIVE_SIDE:
                negative_points++;
                break;
            case CGAL::ON_ORIENTED_BOUNDARY:
                break;
            }
        }
        // we really need to split
        if (positive_points > 0 && negative_points > 0)
        {
            auto& new_facet = facet_split(facet, fold_line);
            facet_mirror(new_facet, fold_line);
        }
        else if (positive_points > 0)
        {
            facet_mirror(facet, fold_line);
        }
    }

    facet& facet_split(facet& old_facet, line fold_line)
    {
        facet facet_positive, facet_negative;
        facet_positive.transform = old_facet.transform;
        facet_negative.transform = old_facet.transform;
        for (size_t segment_id = 0; segment_id < old_facet.size(); segment_id++)
        {
            auto vertex_id = old_facet[segment_id];
            auto segment = destination_segment(old_facet, segment_id);
            auto vertex = segment.source();

            // Not so important check
            assert(vertex == destination_positions[vertex_id]);

            switch (fold_line.oriented_side(vertex))
            {
            case CGAL::ON_POSITIVE_SIDE:
                facet_positive.vertex_ids.push_back(vertex_id);
                break;
            case CGAL::ON_NEGATIVE_SIDE:
                facet_negative.vertex_ids.push_back(vertex_id);
                break;
            case CGAL::ON_ORIENTED_BOUNDARY:
                facet_positive.vertex_ids.push_back(vertex_id);
                facet_negative.vertex_ids.push_back(vertex_id);
                break;
            }
            boost::optional<boost::variant<point, line_segment>> o =
                CGAL::intersection(segment, fold_line);

            if (o != boost::none)
            {
                if (o->which() == 0)
                {
                    // intersection_point is a point
                    auto intersection_point = boost::get<point>(*o);
                    if (intersection_point == segment.target())
                    {
                        // No need for new vertex - vertex id will be added in next iteration.
                    }
                    else
                    {
                        // need new vertex
                        size_t new_vertex_id = destination_positions.size();
                        facet_positive.vertex_ids.push_back(new_vertex_id);
                        facet_negative.vertex_ids.push_back(new_vertex_id);

                        destination_positions.push_back(intersection_point);
                        source_positions.push_back(old_facet.transform(intersection_point));
                    }
                }
                else
                {
                    // intersection is a line
                    assert(false);
                }
            }
        }
        old_facet = facet_negative;
        facets.push_back(facet_positive);
        return facets.back();
    }

    void fold(const line_segment& fold_segment)
    {

        auto verify_fold = [this](const line_segment& fold) {
            int intersections = 0;
            int is_ons = 0;
            int on_segments = 0;
            std::vector<line_segment> segments;
            for (auto facet : facets)
            {
                for (int i = 1; i < facet.size(); i += 1)
                {
                    segments.push_back(
                        line_segment(destination_positions[i - 1], destination_positions[i]));
                }
                segments.push_back(line_segment(destination_positions[facet.size() - 1],
                                                destination_positions[0]));
            }

            for (const auto& s : segments)
            {
                boost::optional<boost::variant<point, line_segment>> o =
                    CGAL::intersection(s, fold);

                if (o == boost::none)
                {
                    continue;
                }

                if (o->which() == 0)
                {
                    intersections += 1;
                    assert(!(s.has_on(fold.source()) && s.has_on(fold.target())));
                    if (s.has_on(fold.source()) || s.has_on(fold.target()))
                    {
                        is_ons += 1;
                    }
                }
                else
                {
                    on_segments += 1;
                }
            }

            if (is_ons == 0)
            {
                logging::error() << "Fold does not start and end on any line segment";
                assert(is_ons > 0);
            }

            if (intersections % 2 != 0)
            {
                logging::warn() << "Fold intersects with an odd number of segments: "
                                << intersections;
            }

            if (on_segments > 0)
            {
                logging::info() << "Fold overlaps with segments: " << on_segments;
            }

            if (is_ons % 2 != 0)
            {
                logging::warn() << "Fold ends/starts on an odd number of segments: " << is_ons;
            }
        };

        verify_fold(fold_segment);

        line fold_line(fold_segment.source(), fold_segment.target());

        size_t old_facet_size = facets.size();
        for (auto facet_id = 0; facet_id < old_facet_size; facet_id++)
        {
            auto& facet = facets[facet_id];

            facet_fold(facet, fold_line);
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

    void to_png(const std::string& prefix) {
        //std::vector<point> source_positions;
        //std::vector<facet> facets;
        //std::vector<point> destination_positions;

        // source points

        auto source_points_dat = prefix + "_source_points.dat";
        std::ofstream os(source_points_dat);
        for (const auto& p : source_positions) {
            os << gmpq_to_double(p.x()) << ' ' << gmpq_to_double(p.y()) << std::endl;
        }
        os.close();

        auto source_points_plot = prefix + "_source_points.plot";
        std::ofstream oss(source_points_plot);
        oss << "set terminal pngcairo size 800,800" << std::endl;
        oss << "set output \"" << prefix << "_source_points.png\"" << std::endl;
        oss << "plot \"" << source_points_dat << "\" ps 3 pt 3";
        oss.close();

        system(("gnuplot " + source_points_plot).c_str());

        unlink(source_points_plot.c_str());
        unlink(source_points_dat.c_str());

        // destination points

        auto destination_points_dat = prefix + "_destination_points.dat";
        std::ofstream od(destination_points_dat);
        for (const auto& p : destination_positions) {
            od << gmpq_to_double(p.x()) << ' ' << gmpq_to_double(p.y()) << std::endl;
        }
        od.close();

        auto destination_points_plot = prefix + "_destination_points.plot";
        std::ofstream odd(destination_points_plot);
        odd << "set terminal pngcairo size 800,800" << std::endl;
        odd << "set output \"" << prefix << "_destination_points.png\"" << std::endl;
        odd << "plot \"" << destination_points_dat << "\" ps 3 pt 3";
        odd.close();

        system(("gnuplot " + destination_points_plot).c_str());

        unlink(destination_points_plot.c_str());
        unlink(destination_points_dat.c_str());

        //for (std::size_t i = 0; i < v.size(); i += 1)
        //{
        //    o << " \"" << f << "_" << i << ".dat\" using 1:2:($3-$1):($4-$2) title \"Polygon_" << i
        //      << "\" with vectors nohead lw 3";
        //for (std::size_t i = 0; i < v.size(); i += 1)
        //{
        //    const auto& polygon = v[i];

        //    std::stringstream s;
        //    s << f << "_" << i << ".dat";

        //    std::ofstream of(s.str());

        //    for (auto v = polygon.edges_begin(); v != polygon.edges_end(); ++v)
        //    {
        //        point a = v->source();
        //        point b = v->target();

        //        of << gmpq_to_double(a.x()) << ' ' << gmpq_to_double(a.y()) << ' '
        //           << gmpq_to_double(b.x()) << ' ' << gmpq_to_double(b.y()) << std::endl;
        //    }

        //    of.close();
        //}

        //std::ofstream o(f + ".plot");

        //o << "set terminal pngcairo size 800,800" << std::endl;
        //o << "set output \"" << f << ".png\"" << std::endl;
        //o << "plot ";

        //for (std::size_t i = 0; i < v.size(); i += 1)
        //{
        //    o << " \"" << f << "_" << i << ".dat\" using 1:2:($3-$1):($4-$2) title \"Polygon_" << i
        //      << "\" with vectors nohead lw 3";

        //    if (i < v.size() - 1)
        //    {
        //        o << ", \\" << std::endl;
        //    }
        //    else
        //    {
        //        o << std::endl;
        //    }
        //}

        //o.close();

        //system(("gnuplot " + f + ".plot").c_str());

        //unlink((f + ".plot").c_str());

        //for (std::size_t i = 0; i < v.size(); i += 1)
        //{
        //    std::stringstream ss;
        //    ss << f << "_" << i << ".dat";
        //    unlink(ss.str().c_str());
        //}
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
        for (auto vertex_id : facet.vertex_ids)
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
