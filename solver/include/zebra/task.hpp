#ifndef __ZEBRA_TASK_HPP__
#define __ZEBRA_TASK_HPP__

#include <iostream>
#include <fstream>
#include <functional>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "geometry.hpp"

namespace zebra {

struct task {

    task() {}

    task(std::vector<polygon> polys, std::vector<line_segment> edgs) :
        polygons{polys}, edges{edgs} {}

    std::vector<polygon> polygons;
    std::vector<line_segment> edges;
};

bool for_n_lines(std::ifstream& in, uint64_t number_of_lines, std::function<bool(const std::string&)> cb)
{
    if (!in) {
        return false;
    }

    for (std::string line; getline(in, line) && number_of_lines > 0; --number_of_lines) {
        if (! cb(line)) {
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
    std::vector<point> points(num_vertices);

    for_n_lines(in, num_vertices, [&](const std::string& line) {
                      points.emplace_back(point_from_string(line));
                      return true;
                });

    return polygon{points.begin(), points.end()};
}


std::vector<polygon> read_polygons(std::ifstream& in, uint64_t num_polygons)
{
    std::vector<polygon> polys;
    while (num_polygons) {
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
    std::vector<line_segment> edges(num_edges);

    for_n_lines(in, num_edges, [&](const std::string& line) {
                //edges.emplace_back(line_segment::from_string(line));
                return true;
            });

    return edges;
}


task read_task(const std::string& taskfile)
{
    std::ifstream in {taskfile};
    assert(in);

    std::string line;
    getline(in, line);
    uint64_t num_polygons = std::stoull(line);

    auto polygons = read_polygons(in, num_polygons);
    auto edges = read_line_segements(in);

    return task {polygons, edges};
}


} /* closing namespace zebra */

#endif /* __ZEBRA_TASK_HPP__ */
