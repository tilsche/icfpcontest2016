#include <zebra/solution.hpp>
using namespace std;
using namespace zebra;

int main(int argc, char** args) {

    srand(time(NULL));

    auto s = solution::unit_square();

    s.to_png("unit_square");

    polygon hull = s.hull();

    for (auto i = hull.edges_begin(); i != hull.edges_end(); ++i) {
        cerr << point_to_string(i->source()) << " " << point_to_string(i->target()) << endl;
    }

    int edge_count = hull.size();

    std::cerr << "edge_count" << edge_count;

    return 0;
}
