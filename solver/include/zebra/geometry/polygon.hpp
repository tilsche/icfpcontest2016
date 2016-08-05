
#ifndef SOLVER_POLYGON_HPP
#define SOLVER_POLYGON_HPP

#include "kernel.hpp"

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

namespace zebra
{
using polygon = CGAL::Polygon_2<kernel>;
using polygon_with_holes = CGAL::Polygon_with_holes_2<kernel>;

inline polygon make_polygon_1()
{
    polygon p;
    p.push_back({0, 0});
    p.push_back({1, 0});
    p.push_back({1, 1});
    p.push_back({0, 1});
    return p;
}

// not tested
void polygons_to_png(const std::vector<polygon>& v, const std::string f) {

    auto t = zebra::read_task(args[1]);

    for (int i = 0; i < v.size(); i += 1) {
        const auto& polygon = v[i];

        stringstream s;
        s << f << "_" << i << ".dat";

        ofstream of(s.str());

        for (auto v = polygon.edges_begin(); v != polygon.edges_end(); ++v) {
            point a = v->source();
            point b = v->target();

            of << gmpq_to_double(a.x()) << ' ' << gmpq_to_double(a.y()) << ' ' << gmpq_to_double(b.x()) << ' ' << gmpq_to_double(b.y()) << endl;
        }

        of.close();
    }

    ofstream o(f + ".plot");

    o << "set terminal pngcairo size 800,800" << endl;
    o << "set output \"" << f << ".png\"" << endl;
    o << "plot ";

    for (int i = 0; i < v.size(); i += 1) {
        o << " \"" << f << "_" << i << ".dat\" using 1:2:($3-$1):($4-$2) title \"Polygon_" << i << "\" with vectors nohead lw 3";

        if (i < v.size()-1) {
            o << ", \\" << endl;
        } else {
            o << endl;
        }
    }

    o.close();

    system("gnuplot " + f + ".plot");

    unlink(f + ".plot");

    for (int i = 0; i < t.sil.polygons.size(); i += 1) {
        stringstream ss;
        ss << f << "_" << i << ".dat";
        unlink(ss.str().c_str());
    }
}

}

#endif // SOLVER_POLYGON_HPP
