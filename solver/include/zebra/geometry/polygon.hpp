
#ifndef SOLVER_POLYGON_HPP
#define SOLVER_POLYGON_HPP

#include "kernel.hpp"

#include <fstream>

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

namespace zebra
{
using polygon = CGAL::Polygon_2<kernel>;
using polygon_with_holes = CGAL::Polygon_with_holes_2<kernel>;

inline polygon make_polygon_1()
{
    polygon p;
    p.push_back({ 0, 0 });
    p.push_back({ 1, 0 });
    p.push_back({ 1, 1 });
    p.push_back({ 0, 1 });
    return p;
}

inline std::vector<point> make_positions_1()
{
    std::vector<point> p;
    p.push_back({ 0, 0 });
    p.push_back({ 1, 0 });
    p.push_back({ 1, 1 });
    p.push_back({ 0, 1 });
    return p;
}

// not tested
void polygons_to_png(const std::vector<polygon>& v, const std::string f)
{

    for (std::size_t i = 0; i < v.size(); i += 1)
    {
        const auto& polygon = v[i];

        std::stringstream s;
        s << f << "_" << i << ".dat";

        std::ofstream of(s.str());

        for (auto v = polygon.edges_begin(); v != polygon.edges_end(); ++v)
        {
            point a = v->source();
            point b = v->target();

            of << gmpq_to_double(a.x()) << ' ' << gmpq_to_double(a.y()) << ' '
               << gmpq_to_double(b.x()) << ' ' << gmpq_to_double(b.y()) << std::endl;
        }

        of.close();
    }

    std::ofstream o(f + ".plot");

    o << "set terminal pngcairo size 800,800" << std::endl;
    o << "set output \"" << f << ".png\"" << std::endl;
    o << "plot ";

    for (std::size_t i = 0; i < v.size(); i += 1)
    {
        o << " \"" << f << "_" << i << ".dat\" using 1:2:($3-$1):($4-$2) title \"Polygon_" << i
          << "\" with vectors nohead lw 3";

        if (i < v.size() - 1)
        {
            o << ", \\" << std::endl;
        }
        else
        {
            o << std::endl;
        }
    }

    o.close();

    system(("gnuplot " + f + ".plot").c_str());

    unlink((f + ".plot").c_str());

    for (std::size_t i = 0; i < v.size(); i += 1)
    {
        std::stringstream ss;
        ss << f << "_" << i << ".dat";
        unlink(ss.str().c_str());
    }
}
}

#endif // SOLVER_POLYGON_HPP
