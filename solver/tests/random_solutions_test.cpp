#include <cstdlib>

#include <zebra/solution.hpp>
using namespace std;
using namespace zebra;

int rand(int off, int size_) {
    return off + (rand() % (size_));
}

CGAL::Gmpq rand(CGAL::Gmpq off, CGAL::Gmpq size_) {
    return off + size_ / rand(1, 10);
}

point rand(point a, point b, int parts) {
    CGAL::Vector_2<kernel> v(a, b);
    v = v / parts;
    return a + v + v*rand(0, parts-1);
}

line_segment random_fold(const solution& s) {
    polygon hull = s.hull();

    //for (auto i = hull.edges_begin(); i != hull.edges_end(); ++i) {
    //    cerr << point_to_string(i->source()) << " " << point_to_string(i->target()) << endl;
    //}

    int edge_count = hull.size();

    //std::cerr << "edge_count" << edge_count << endl;

    int e1 = rand(0, edge_count);
    int e2 = e1;

    while (e2 == e1) {
        e2 = rand(0, edge_count);
    }

    //std::cerr << "edge 1: " << e1 << endl;
    //std::cerr << "edge 2: " << e2 << endl;

    line_segment s1 = hull.edge(e1);
    line_segment s2 = hull.edge(e2);

    point p1 = rand(s1.source(), s1.target(), 10);
    point p2 = rand(s2.source(), s2.target(), 10);

    std::cerr << "point 1: " << point_to_string(p1) << endl;
    std::cerr << "point 2: " << point_to_string(p2) << endl;

    return line_segment(p1, p2);
}

int main(int argc, char** args) {

    unsigned int seed;
    unsigned int repeat;

    if (argc < 2) {
        seed = time(NULL);
    } else {
        seed = atoi(args[1]);
    }

    if (argc < 3) {
        repeat = 1;
    } else {
        repeat = atoi(args[2]);
    }

    srand(seed);

    auto s = solution::unit_square();

    stringstream name_;
    name_ << "seed_" << seed << "_" << 0;
    auto name = name_.str();

    s.to_png(name);


    string previous_name = "";
    string previous_solution = "";

    for (int i = 1; i <= repeat; i += 1) {
        stringstream name_;
        name_ << "seed_" << seed << "_" << i;
        auto name = name_.str();

        line_segment f = random_fold(s);

        s.fold(f);

        s.to_png(name);

        stringstream ss;
        ss << s;

        if (ss.str().size() > 5000) {
            logging::error() << "repetition: " << i << ", solution size > 5000";

            std::cout << endl << endl;
            std::cout << "== " << previous_name << " == size: " << previous_solution.size() << " ==" << endl;
            std::cout << previous_solution;
            std::cout << endl;
            return 0;
        }

        previous_name = name;
        previous_solution = ss.str();

        std::cout << endl << endl;
        std::cout << "== " << previous_name << " == size: " << previous_solution.size() << " ==" << endl;
        std::cout << previous_solution;
        std::cout << endl;

    }

    return 0;
}
