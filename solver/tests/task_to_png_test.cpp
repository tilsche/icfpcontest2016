#include <zebra/task.hpp>
#include <cassert>
#include <fstream>
#include <libgen.h>

using namespace std;
using namespace zebra;

int main(int argc, const char** args)
{
    assert(argc >= 2);

    auto t = zebra::read_task(args[1]);

    for (std::size_t i = 0; i < t.sil.polygons.size(); i += 1) {
        const auto& polygon = t.sil.polygons[i];

        stringstream s;
        s << "sil_poly_" << i << ".dat";

        ofstream f(s.str());

        for (auto v = polygon.edges_begin(); v != polygon.edges_end(); ++v) {
            point a = v->source();
            point b = v->target();

            f << gmpq_to_double(a.x()) << ' ' << gmpq_to_double(a.y()) << ' ' << gmpq_to_double(b.x()) << ' ' << gmpq_to_double(b.y()) << endl;
        }

        f.close();

    }

    ofstream skelo("skel.dat");
    for (const auto& l : t.skel.edges) {
        point a = l.source();
        point b = l.target();

        skelo << gmpq_to_double(a.x()) << ' ' << gmpq_to_double(a.y()) << ' ' << gmpq_to_double(b.x()) << ' ' << gmpq_to_double(b.y()) << endl;
    }
    skelo.close();

    char *f = strdup(args[1]);

    ofstream o("a.plot");

    o << "set terminal pngcairo size 800,800" << endl;
    o << "set output \"" << basename(f) << ".png\"" << endl;
    o << "plot ";

    for (std::size_t i = 0; i < t.sil.polygons.size(); i += 1) {
        o << "\"sil_poly_" << i << ".dat\" using 1:2:($3-$1):($4-$2) title \"Silhouette_Polygon" << i << "\" with vectors nohead lw 3, \\" << endl;
    }

    o << " \"skel.dat\" using 1:2:($3-$1):($4-$2) title \"Skeleton\" with vectors nohead lw 3" << endl;

    o.close();

    free(f);

    system("gnuplot a.plot");

    unlink("a.plot");

    for (std::size_t i = 0; i < t.sil.polygons.size(); i += 1) {
        stringstream ss;
        ss << "sil_poly_" << i << ".dat";
        unlink(ss.str().c_str());
    }
    unlink("skel.dat");

    return 0;
}
