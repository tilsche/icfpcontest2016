
#ifndef SOLVER_SOLUTION_HPP
#define SOLVER_SOLUTION_HPP

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include <zebra/geometry.hpp>
#include <zebra/log.hpp>
#include <zebra/silhouette.hpp>

#include <CGAL/Boolean_set_operations_2.h>

namespace zebra
{

class facet
{
public:
    facet()
    {
    }

    facet(transformation t) : transform_(t)
    {
    }

    static facet unit_facet()
    {
        facet f;
        f.vertex_ids = { 0, 1, 2, 3 };
        return f;
    }

    size_t size() const
    {
        return vertex_ids.size();
    }

    size_t operator[](size_t index) const
    {
        return vertex_ids[index % size()];
    }

    std::vector<size_t> vertex_ids;

    transformation transform() const
    {
        return transform_;
    }

    transformation inverse() const
    {
        transformation i = transform_.inverse();
        logging::trace() << "INVERTING: " << transform_;
        logging::trace() << " = : " << i;
        logging::trace() << "ID: " << transform_ * i;
        return i;
    }

    void transform(transformation t)
    {
        logging::trace() << "Adding transformation: " << t << " to " << transform_;
        //transform_ = transform_ * t;
        transform_ = t * transform_;
        logging::trace() << "= " << transform_;
    }

private:
    transformation transform_;
};

struct solution
{
    std::vector<point> source_positions;
    std::vector<facet> facets;
    std::vector<point> destination_positions;
    std::map<point, size_t> source_positions_by_point;

    size_t add_positions(point source_point, point destination_point)
    {
        assert(source_positions.size() == destination_positions.size());
        auto new_idx = source_positions.size();

        auto try_insert = source_positions_by_point.emplace(std::make_pair(source_point, new_idx));
        if (try_insert.second)
        {
            destination_positions.push_back(destination_point);
            source_positions.push_back(source_point);
            return new_idx;
        }
        else
        {
            auto old_id = try_insert.first->second;
            assert(destination_positions[old_id] == destination_point);
            return old_id;
        }
    }

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
        auto s = line_segment(destination_positions[facet[segment_id]],
                              destination_positions[facet[segment_id + 1]]);
        if (s.source() == s.target())
        {
            logging::error() << "Invalid destination segment: " << s;
        }
        return s;
    }

    line_segment source_segment(const facet& facet, size_t segment_id) const
    {
        assert(segment_id < facet.size());
        auto s = line_segment(source_positions[facet[segment_id]],
                              source_positions[facet[segment_id + 1]]);
        if (s.source() == s.target())
        {
            logging::error() << "Invalid destination segment: " << s;
        }
        return s;
    }

    void facet_mirror(facet& facet, line fold_line)
    {
        // DOES NOT YET MIRROR THE POINTS
        auto mirror = reflection(fold_line);
        facet.transform(mirror);
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
        else if (negative_points > 0)
        {
            facet_mirror(facet, fold_line);
        }
    }

    facet& facet_split(facet& old_facet, line fold_line)
    {
        facet facet_positive(old_facet.transform());
        facet facet_negative(old_facet.transform());
        for (size_t segment_id = 0; segment_id < old_facet.size(); segment_id++)
        {
            auto vertex_id = old_facet[segment_id];
            auto segment = destination_segment(old_facet, segment_id);
            auto vertex = segment.source();

            // Not so important check
            assert(vertex == destination_positions[vertex_id]);

            logging::debug() << "vertex " << vertex_id << "@" << vertex << ", segment " << segment;
            switch (fold_line.oriented_side(vertex))
            {
            case CGAL::ON_POSITIVE_SIDE:
                logging::debug() << " POSITIVE";
                facet_positive.vertex_ids.push_back(vertex_id);
                break;
            case CGAL::ON_NEGATIVE_SIDE:
                logging::debug() << "NEGATVE";
                facet_negative.vertex_ids.push_back(vertex_id);
                break;
            case CGAL::ON_ORIENTED_BOUNDARY:
                logging::debug() << "BOUNDARY";
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
                    if (intersection_point == segment.target() ||
                        intersection_point == segment.source())
                    {
                        logging::debug() << "Intersecting at line ";
                        // No need for new vertex - vertex id will be added in next iteration.
                    }
                    else
                    {
                        // need new vertex
                        auto source_point = old_facet.inverse()(intersection_point);
                        auto new_vertex_id = add_positions(source_point, intersection_point);
                        logging::debug() << "new vertex " << new_vertex_id << "@"
                                         << intersection_point;
                        facet_positive.vertex_ids.push_back(new_vertex_id);
                        facet_negative.vertex_ids.push_back(new_vertex_id);
                    }
                }
                else
                {
                    // intersection is a line
                    logging::error() << "Intersection of segment " << segment << " on fold "
                                     << fold_line << " is a  line.";
                    assert(false);
                }
            }
        }
        old_facet = facet_positive;
        facets.push_back(facet_negative);
        return facets.back();
    }

    polygon facet_poly(const facet& f) const
    {
        polygon p;
        for (auto i : f.vertex_ids)
        {
            p.push_back(destination_positions[i]);
        }
        if (p.is_counterclockwise_oriented())
        {
            return p;
        }
        p = polygon();
        for (auto it = f.vertex_ids.rbegin(); it != f.vertex_ids.rend(); it++)
        {
            auto id = *it;
            p.push_back(destination_positions[id]);
        }
        assert(p.is_counterclockwise_oriented());
        return p;
    }

    void fold(const line_segment& fold_segment)
    {

        // auto verify_fold = [this](const line_segment& fold) {
        //    int intersections = 0;
        //    int is_ons = 0;
        //    int on_segments = 0;
        //    std::vector<line_segment> segments;
        //    for (auto facet : facets)
        //    {
        //        for (int i = 1; i < facet.size(); i += 1)
        //        {
        //            segments.push_back(
        //                line_segment(destination_positions[i - 1], destination_positions[i]));
        //        }
        //        segments.push_back(line_segment(destination_positions[facet.size() - 1],
        //                                        destination_positions[0]));
        //    }

        //    for (const auto& s : segments)
        //    {
        //        boost::optional<boost::variant<point, line_segment>> o =
        //            CGAL::intersection(s, fold);

        //        if (o == boost::none)
        //        {
        //            continue;
        //        }

        //        if (o->which() == 0)
        //        {
        //            intersections++;
        //            assert(!(s.has_on(fold.source()) && s.has_on(fold.target())));
        //            if (s.has_on(fold.source()) || s.has_on(fold.target()))
        //            {
        //                is_ons++;
        //            }
        //        }
        //        else
        //        {
        //            on_segments++;
        //        }
        //    }

        //    if (is_ons == 0)
        //    {
        //        logging::error() << "Fold does not start and end on any line segment";
        //        assert(is_ons > 0);
        //    }

        //    if (intersections % 2 != 0)
        //    {
        //        logging::warn() << "Fold intersects with an odd number of segments: "
        //                        << intersections;
        //    }

        //    if (on_segments > 0)
        //    {
        //        logging::info() << "Fold overlaps with segments: " << on_segments;
        //    }

        //    if (is_ons % 2 != 0)
        //    {
        //        logging::warn() << "Fold ends/starts on an odd number of segments: " << is_ons;
        //    }
        //};

        // verify_fold(fold_segment);

        line fold_line(fold_segment.source(), fold_segment.target());

        size_t old_facet_size = facets.size();
        for (auto facet_id = 0; facet_id < old_facet_size; facet_id++)
        {
            auto& facet = facets[facet_id];

            facet_fold(facet, fold_line);
        }
        auto mirror = reflection(fold_line);
        for (auto& destination_position : destination_positions)
        {
            if (fold_line.has_on_negative_side(destination_position))
            {
                destination_position = mirror(destination_position);
            }
        }
    }

    void transform(transformation t)
    {
        for (auto& f : facets)
        {
            f.transform(t);
        }
        for (auto& dp : destination_positions)
        {
            dp = t(dp);
        }
    }

    void to_png(const std::string& prefix)
    {
        std::vector<std::string> unlink_files;
        // source points

        auto source_points_dat = prefix + "_source_points.dat";
        std::ofstream os(source_points_dat);
        unlink_files.push_back(source_points_dat);
        for (const auto& p : source_positions)
        {
            os << gmpq_to_double(p.x()) << ' ' << gmpq_to_double(p.y()) << std::endl;
        }
        os.close();

        auto source_plot = prefix + "_source.plot";
        std::ofstream oss(source_plot);
        unlink_files.push_back(source_plot);
        oss << "set terminal pngcairo size 800,800" << std::endl;
        oss << "set output \"" << prefix << "_source.png\"" << std::endl;
        oss << "set size square" << std::endl;
        oss << "plot \"" << source_points_dat << "\" ps 3 pt 3, \\" << std::endl;

        // source facets

        for (int i = 0; i < facets.size(); i += 1)
        {
            auto facet = facets[i].vertex_ids;
            facet.push_back(facet[0]);

            std::stringstream facet_dats;
            facet_dats << prefix << "_source_facet_" << i << ".dat";
            auto facet_dat = facet_dats.str();

            unlink_files.push_back(facet_dat);

            std::ofstream of(facet_dat);
            for (int j = 0; j < facet.size(); j += 1)
            {
                auto p = source_positions[facet[j]];
                of << gmpq_to_double(p.x()) << ' ' << gmpq_to_double(p.y()) << std::endl;
            }
            of.close();

            oss << "\"" << facet_dat << "\" with lines lw 3";

            if (i < facets.size() - 1)
            {
                oss << ", \\" << std::endl;
            }
            else
            {
                oss << std::endl;
            }
        }

        oss.close();

        system(("gnuplot " + source_plot).c_str());

        // destination plot

        auto destination_points_dat = prefix + "_target.dat";
        std::ofstream od(destination_points_dat);
        unlink_files.push_back(destination_points_dat);
        for (const auto& p : destination_positions)
        {
            od << gmpq_to_double(p.x()) << ' ' << gmpq_to_double(p.y()) << std::endl;
        }
        od.close();

        auto destination_plot = prefix + "_target.plot";
        std::ofstream odd(destination_plot);
        unlink_files.push_back(destination_plot);
        odd << "set terminal pngcairo size 800,800" << std::endl;
        odd << "set output \"" << prefix << "_target.png\"" << std::endl;
        oss << "set size square" << std::endl;
        odd << "plot \"" << destination_points_dat << "\" ps 3 pt 3, \\" << std::endl;

        // destination facets

        for (int i = 0; i < facets.size(); i += 1)
        {
            auto facet = facets[i].vertex_ids;
            facet.push_back(facet[0]);

            std::stringstream facet_dats;
            facet_dats << prefix << "_destination_facet_" << i << ".dat";
            auto facet_dat = facet_dats.str();

            unlink_files.push_back(facet_dat);

            std::ofstream of(facet_dat);
            for (int j = 0; j < facet.size(); j += 1)
            {
                auto p = destination_positions[facet[j]];
                of << gmpq_to_double(p.x()) << ' ' << gmpq_to_double(p.y()) << std::endl;
            }
            of.close();

            odd << "\"" << facet_dat << "\" with lines lw 3";

            if (i < facets.size() - 1)
            {
                odd << ", \\" << std::endl;
            }
            else
            {
                odd << std::endl;
            }
        }

        odd.close();

        system(("gnuplot " + destination_plot).c_str());

        for (const auto& f : unlink_files)
        {
            unlink(f.c_str());
        }
    }

    static solution unit_square()
    {
        solution ret;
        ret.source_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
        ret.facets = { facet::unit_facet() };
        ret.destination_positions = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
        return ret;
    }

    double resemblance(polygon target) const
    {
        logging::trace() << "calculating resemblence with " << target;
        logging::trace() << "solution: " << *this;
        polygon_with_holes p_or;
        polygon_with_holes solution_poly = poly();
        logging::trace() << "solution_poly: " << solution_poly;
        std::vector<polygon_with_holes> p_ands;
        CGAL::join(solution_poly, target, p_or);
        CGAL::intersection(solution_poly, target, std::back_inserter(p_ands));
        CGAL::Gmpq and_area = 0;
        for (const auto& holy : p_ands)
        {
            and_area += holy.outer_boundary().area();
        }
        auto or_area = p_or.outer_boundary().area();
        return gmpq_to_double(and_area / or_area);
    }

    polygon_with_holes poly() const
    {

        polygon_with_holes ret;

        bool first = true;
        for (const facet& f : facets)
        {
            auto p = facet_poly(f);

            if (first == true)
            {
                CGAL::join(p, p, ret);
                first = false;
            }

            polygon_with_holes tmp;

            CGAL::join(p, ret, tmp);

            ret = tmp;

            // std::cerr << "facet: ";
            // for (int i = 0; i < f.vertex_ids.size(); i += 1) {
            //    std::cerr << f.vertex_ids[i] << ", ";
            //}
            // std::cerr << std::endl;
        }

        return ret;
    }

    polygon hull() const
    {
        return poly().outer_boundary();
    }

    // double resemblance(const silhouette& them__) const
    // {

    //      auto to_silhoutte = [this]() -> vector<polygon> const {
    //         vector<polygon> ret;

    //         for (const facet& f, this->facets) {

    //             polygon p;
    //             for (int i : f.vertex_ids) {
    //                 p.push_back(destination_positions[i]);
    //             }
    //             ret.push_back(p);
    //         }
    //         return ret;
    //     }

    //     auto to_polygon = [](const std::vector<polygon>& polygons) {

    //         polygon_with_holes polygon;
    //         polygon_with_holes tmp;

    //         int j = 0;
    //         while (polygons[j].is_counterclockwise_oriented() == false()) {
    //             j += 1;
    //         }

    //         if (j == polygons.size()) {
    //             std::cerr << "no positive size polygons";
    //             assert(j == polygons.size());
    //         }

    //         polygon = polygons[j];

    //         for (int i = 0; i < polygons.size(); i += 1) {

    //             if (i == j) { continue; }

    //             if (p.is_counterclockwise_oriented()) {
    //                 CGAL::join(target, p, tmp);
    //             } else {
    //                 std::vector<polygon_with_holes> v;
    //                 CGAL::intersection(target, p, begin(v));

    //             }
    //             target = tmp;
    //         }

    //         return polygon;

    //     }

    //     const auto& them_ = them__.polygons;
    //     const auto& us_ = to_silhoutte();

    //     polygon_with_holes them = to_polygon(them_);
    //     polygon_with_holes us   = to_polygon(us_);

    //     polygon_with_holes both;
    //     CGAL::join(target)

    // //double resemblance(polygon target) const
    // //{
    // //    polygon_with_holes p_or;
    // //    std::vector<polygon_with_holes> p_ands;
    // //    CGAL::join(poly, target, p_or);
    // //    CGAL::intersection(poly, target, std::back_inserter(p_ands));
    // //    CGAL::Gmpq and_area = 0;
    // //    for (const auto& holy : p_ands)
    // //    {
    // //        and_area += holy.outer_boundary().area();
    // //    }
    // //    auto or_area = p_or.outer_boundary().area();
    // //    return gmpq_to_double(and_area / or_area);
    // //}
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
