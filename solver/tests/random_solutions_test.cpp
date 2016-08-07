#include <cstdlib>

#include <zebra/log.hpp>
#include <zebra/solution.hpp>

using namespace zebra;

int rand(int off, int size_)
{
    return off + (rand() % (size_));
}

CGAL::Gmpq rand(CGAL::Gmpq off, CGAL::Gmpq size_)
{
    return off + size_ / rand(1, 10);
}

point rand(point a, point b, int parts)
{
    CGAL::Vector_2<kernel> v(a, b);
    v = v / parts;
    return a + v + v * rand(0, parts - 1);
}

void random_fold(solution& s)
{
    auto fid = rand(0, s.facet_size());
    auto f = s.get_facet(fid);

    int e1 = rand(0, f.size());
    int e2 = e1;

    while (e2 == e1)
    {
        e2 = rand(0, f.size());
    }

    auto s1 = s.destination_segment(f, e1);
    auto s2 = s.destination_segment(f, e2);

    point p1 = rand(s1.source(), s1.target(), 10);
    point p2 = rand(s2.source(), s2.target(), 10);

    std::cerr << "point 1: " << point_to_string(p1) << std::endl;
    std::cerr << "point 2: " << point_to_string(p2) << std::endl;

    s.fold(fid, line_segment(p1, p2));
    s.rotate(p1, line_segment(p1, p2));
    s.translate(p2);
}

int main(int argc, char** args)
{

    unsigned int seed;
    unsigned int repeat;

    if (argc < 2)
    {
        seed = time(NULL);
    }
    else
    {
        seed = atoi(args[1]);
    }

    if (argc < 3)
    {
        repeat = 1;
    }
    else
    {
        repeat = atoi(args[2]);
    }

    srand(seed);

    auto s = solution::unit_square();

    std::stringstream name_;
    name_ << "seed_" << seed << "_" << 0;
    auto name = name_.str();

    s.to_png(name);

    std::string previous_name = "";
    std::string previous_solution = "";

    for (int i = 1; i <= repeat; i += 1)
    {
        std::stringstream name_;
        name_ << "seed_" << seed << "_" << i;
        auto name = name_.str();

        random_fold(s);

        s.to_png(name);

        std::stringstream ss;
        ss << s;

        if (ss.str().size() > 5000)
        {
            logging::error() << "repetition: " << i << ", solution size > 5000";

            std::cout << std::endl << std::endl;
            std::cout << "== " << previous_name << " == size: " << previous_solution.size()
                      << " ==" << std::endl;
            std::cout << previous_solution;
            std::cout << std::endl;
            return 0;
        }

        previous_name = name;
        previous_solution = ss.str();

        std::cout << std::endl << std::endl;
        std::cout << "== " << previous_name << " == size: " << previous_solution.size()
                  << " ==" << std::endl;
        std::cout << previous_solution;
        std::cout << std::endl;
    }

    return 0;
}
