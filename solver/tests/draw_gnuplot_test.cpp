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

    ofstream silo("sil.dat");
    for (const auto& polygon : t.sil.polygons) {

        for (auto v = polygon.edges_begin(); v != polygon.edges_end(); ++v) {
            point a = v->source();
            point b = v->target();

            silo << gmpq_to_double(a.x()) << ' ' << gmpq_to_double(a.y()) << ' ' << gmpq_to_double(b.x()) << ' ' << gmpq_to_double(b.y()) << endl;
        }

    }

    ofstream skelo("skel.dat");
    for (const auto& l : t.skel.edges) {
        point a = l.source();
        point b = l.target();

        skelo << gmpq_to_double(a.x()) << ' ' << gmpq_to_double(a.y()) << ' ' << gmpq_to_double(b.x()) << ' ' << gmpq_to_double(b.y()) << endl;
    }

    char *f = strdup(args[1]);

    ofstream o("a.plot");

    o << "set terminal pngcairo size 1280,800" << endl;
    o << "set output \"" << basename(f) << ".png\"" << endl;
    o << "plot \"sil.dat\" using 1:2:($3-$1):($4-$2) title \"Silhouette\" with vectors nohead lw 3, \\" << endl;
    o << "     \"skel.dat\" using 1:2:($3-$1):($4-$2) title \"Skeleton\" with vectors nohead lw 3" << endl;

    o.close();

    free(f);

    system("gnuplot a.plot");

    unlink("sil.dat");
    unlink("skel.dat");

    return 0;
}
