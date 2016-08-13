#ifndef __ZEBRA_TASK_HPP__
#define __ZEBRA_TASK_HPP__

#include "geometry.hpp"
#include "silhouette.hpp"
#include "skeleton.hpp"
#include <zebra/log.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

namespace zebra
{

struct task
{
    task(std::vector<polygon> polys, std::vector<line_segment> edgs) : sil{ polys }, skel{ edgs }
    {
    }

    silhouette sil;
    skeleton skel;

    std::vector<polygon> to_polygon_vector() const
    {

        // ### gather all points

        std::map<point, int /* number of incident edges */> all_points;

        for (const auto& poly : sil.polygons())
        {
            for (auto p = poly.vertices_begin(); p != poly.vertices_end(); ++p)
            {
                all_points.insert(std::make_pair(*p, 0));
            }
        }

        // ### determine extra points from edge intersections

        std::vector<line_segment> all_segments = skel.edges;

        for (const auto& s : all_segments)
        {
            for (const auto& t : all_segments)
            {
                boost::optional<boost::variant<point, line_segment>> o = CGAL::intersection(s, t);
                if (o != boost::none && o->which() == 0)
                {
                    all_points.insert(std::make_pair(boost::get<point>(*o), 0));
                }
            }
        }

        // ### split edges on new points

        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto s = begin(all_segments); s != end(all_segments); ++s)
            {
                for (const auto& p : all_points)
                {
                    if (s->has_on(p.first) && p.first != s->source() && p.first != s->target())
                    {
                        auto t = *s;
                        all_segments.erase(s);
                        all_segments.push_back(line_segment(t.source(), p.first));
                        all_segments.push_back(line_segment(p.first, t.target()));
                        changed = true;
                        break;
                    }
                }

                if (changed)
                {
                    break;
                }
            }
        }

        auto segments_on_point = [](const std::vector<line_segment>& segments,
                                    const std::map<point, int>& valid_points,
                                    const point& p) -> std::vector<line_segment> {
            std::vector<line_segment> ret;
            for (const auto& s : segments)
            {
                if (s.has_on(p) && valid_points.find(s.source()) != valid_points.end() &&
                    valid_points.find(s.target()) != valid_points.end())
                {
                    ret.push_back(s);
                }
            }
            return ret;
        };

        // ### count incident line segments

        for (auto&& p : all_points)
        {
            for (const auto& s : all_segments)
            {
                if (s.has_on(p.first))
                {
                    p.second += 1;
                }
            }
        }

        int j = 1;
        std::cerr << "all segments: " << std::endl;
        for (const auto& s : all_segments)
        {
            std::cerr << j << " " << line_segment_to_string(s) << std::endl;
            j += 1;
        }
        std::cerr << std::endl;

        std::vector<polygon> ret;

        std::string asdf = "0";
        while (all_points.empty() == false)
        {

            std::cerr << "all points: " << std::endl;
            int i = 1;
            for (const auto& p : all_points)
            {
                std::cerr << i << " " << point_to_string(p.first) << ": " << p.second << std::endl;
                i += 1;
            }
            std::cerr << std::endl;

            // ### new polygon: add first point and line segment ###

            std::vector<point> v{ all_points.begin()->first };
            std::cerr << "first point of new polygon: " << point_to_string(v[0]) << ": "
                      << all_points.begin()->second << std::endl;
            all_points.begin()->second -= 1;

            if (all_points.begin()->second == 0)
            {
                all_points.erase(all_points.begin());
            }

            std::vector<line_segment> segments =
                segments_on_point(all_segments, all_points, v.back());

            // std::cerr << "incident line segments: " << std::endl;
            // for (const auto& s : segments) {
            //    std::cerr << "  " << line_segment_to_string(s) << std::endl;
            //}

            line_segment s = segments[0];
            if (s.target() == v.back())
            {
                s = s.opposite();
            }

            v.push_back(s.target());
            all_points[v.back()] -= 1;
            if (all_points[v.back()] == 0)
            {
                all_points.erase(v.back());
            }

            auto last_segment = s;
            auto last_segment_dir = CGAL::Direction_2<kernel>(last_segment.opposite());

            // ### add all other points

            while (v.front() != v.back() || v.size() == 1)
            {

                std::vector<line_segment> segments =
                    segments_on_point(all_segments, all_points, v.back());

                // std::cerr << "incident line segments: " << std::endl;
                // for (const auto& s : segments) {
                //    std::cerr << "  " << line_segment_to_string(s) << std::endl;
                //}

                line_segment next_segment;

                for (auto s : segments)
                {
                    if (s == last_segment || s == last_segment.opposite())
                    {
                        continue;
                    }
                    if (s.target() == v.back())
                    {
                        s = s.opposite();
                    }
                    auto s_dir = CGAL::Direction_2<kernel>(s);

                    bool correct_one = true;
                    for (auto t : segments)
                    {
                        if (t == last_segment || t == last_segment.opposite())
                        {
                            continue;
                        }
                        if (t.target() == v.back())
                        {
                            t = t.opposite();
                        }
                        auto t_dir = CGAL::Direction_2<kernel>(t);

                        if (last_segment_dir.counterclockwise_in_between(s_dir, t_dir) == false)
                        {
                            correct_one = false;
                            break;
                        }
                    }

                    if (correct_one == true)
                    {
                        next_segment = s;
                        break;
                    }
                }

                // std::cerr << "next segment: " << line_segment_to_string(next_segment) <<
                // std::endl;

                last_segment = next_segment;
                last_segment_dir = CGAL::Direction_2<kernel>(last_segment.opposite());
                v.push_back(next_segment.target());
                all_points[v.back()] -= 1;
                if (all_points[v.back()] == 0)
                {
                    all_points.erase(v.back());
                }

                std::cerr << "current polygon: " << std::endl;
                int k = 1;
                for (const auto& p : v)
                {
                    std::cerr << k << " " << point_to_string(p) << std::endl;
                    k += 1;
                }
                std::cerr << std::endl;

                std::cerr << "all points: " << std::endl;
                int i = 1;
                for (const auto& p : all_points)
                {
                    std::cerr << i << " " << point_to_string(p.first) << ": " << p.second
                              << std::endl;
                    i += 1;
                }
                std::cerr << std::endl;
            }

            ret.push_back(polygon(v.begin(), v.end()));

            polygons_to_png(ret, asdf);
            asdf += "0";

            break;
        }

        return ret;
    }
};

bool for_n_lines(std::ifstream& in, uint64_t number_of_lines,
                 std::function<bool(const std::string&)> cb)
{
    if (!in)
    {
        return false;
    }

    for (std::string line; number_of_lines > 0; --number_of_lines)
    {
        // TODO error check
        getline(in, line);
        if (!cb(line))
        {
            break;
        }
    }

    return true;
}

polygon read_polygon(std::ifstream& in)
{
    std::string line;
    getline(in, line);
    uint64_t num_vertices = std::stoull(line);
    std::vector<point> points;
    points.reserve(num_vertices);

    for_n_lines(in, num_vertices, [&](const std::string& line) {
        points.emplace_back(point_from_string(line));
        return true;
    });

    return polygon{ points.begin(), points.end() };
}

std::vector<polygon> read_polygons(std::ifstream& in, uint64_t num_polygons)
{
    std::vector<polygon> polys;
    while (num_polygons)
    {
        polys.emplace_back(read_polygon(in));
        --num_polygons;
    }

    return polys;
}

std::vector<line_segment> read_line_segements(std::ifstream& in)
{
    std::string line;
    getline(in, line);
    uint64_t num_edges = std::stoull(line);
    std::vector<line_segment> edges;
    edges.reserve(num_edges);

    for_n_lines(in, num_edges, [&](const std::string& line) {
        edges.emplace_back(line_segment_from_string(line));
        return true;
    });

    return edges;
}

task read_task(const std::string& taskfile)
{
    zebra::logging::info() << "Reading task from: " << taskfile;

    std::ifstream in{ taskfile };
    assert(in);

    std::string line;
    getline(in, line);
    uint64_t num_polygons = std::stoull(line);

    auto polygons = read_polygons(in, num_polygons);
    auto edges = read_line_segements(in);

    return task{ polygons, edges };
}

} /* closing namespace zebra */

#endif /* __ZEBRA_TASK_HPP__ */
