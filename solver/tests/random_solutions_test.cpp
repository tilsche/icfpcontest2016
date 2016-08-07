#include <cstdlib>

#include <zebra/log.hpp>
#include <zebra/pythagoras.hpp>
#include <zebra/solution.hpp>

#include <random>

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

point evil_point(point a, point b, int parts)
{
    CGAL::Vector_2<kernel> v(a, b);
    v = v / parts;
    return a + v;
}

auto& get_triple()
{
    static auto triples = zebra::get_pythagorean_triples();

    return triples[rand(0, int(triples.size()))];
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

    // std::cerr << "point 1: " << point_to_string(p1) << std::endl;
    // std::cerr << "point 2: " << point_to_string(p2) << std::endl;

    s.fold(fid, line_segment(p1, p2));
}

void evil_fold(solution& s)
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

    point p1 = s1.source();
    point p2 = evil_point(s2.source(), s2.target(), 100);

    // std::cerr << "point 1: " << point_to_string(p1) << std::endl;
    // std::cerr << "point 2: " << point_to_string(p2) << std::endl;

    s.fold(fid, line_segment(p1, p2));
}

int main(int argc, char** args)
{

    unsigned int seed;
    unsigned int repeat;

    if (argc < 2)
    {
        std::random_device rd;

        seed = rd();
    }
    else
    {
        seed = atoi(args[1]);
    }

    if (argc < 3)
    {
        repeat = 6;
    }
    else
    {
        repeat = atoi(args[2]);
    }

    srand(seed);

    auto s = solution::unit_square();

    // std::stringstream name_;
    // name_ << "seed_" << seed << "_" << 0;
    // auto name = name_.str();

    // s.to_png(name);

    std::string previous_name = "";
    std::string previous_solution = "";

    bool stop = false;

    for (int i = 1; i <= repeat; i += 1)
    {
        // std::stringstream name_;
        // name_ << "seed_" << seed << "_" << i;
        // auto name = name_.str();

        random_fold(s);

        // s.to_png(name);

        if (s.file_size() > 5000)
        {
            logging::error() << "repetition: " << i << ", solution size > 5000";

            stop = true;
        }
        else
        {
            // std::cout << "== " << previous_name << " == size: " << previous_solution.size()
            //   << " ==" << std::endl;
        }

        // std::stringstream ss;
        // ss << s;

        // previous_name = name;
        // previous_solution = ss.str();

        // std::cout << std::endl << std::endl;
        // std::cout << previous_solution;
        // std::cout << std::endl;

        if (stop)
        {
            break;
        }
    }

    auto triple = get_triple();

    s.rotate(point(-1, -1), line_segment(point(0, 0), point(triple[0], triple[1])));
    evil_fold(s);

    std::stringstream ss;

    {
        std::stringstream name_;
        name_ << "seed_" << seed << "_final";
        auto name = name_.str();

        // s.to_png(name);

        ss << s;

        previous_name = name;
    }
    previous_solution = ss.str();

    // std::cout << "== " << previous_name
    //   << "== applying random rotation and evil2 fold == size:" << s.file_size()
    //   << " ==" << std::endl;
    // std::cout << std::endl << std::endl;
    // std::cout << previous_solution;
    // std::cout << std::endl;

    if (s.file_size() < 1000)
    {
        std::cout << "== " << previous_name
                  << "== applying random rotation and evil2 fold == size:" << s.file_size();
        std::cout << std::endl << std::endl;
        std::cout << previous_solution;
        std::cout << std::endl;

        std::stringstream name_;
        name_ << "seed_" << seed << "_final";
        auto name = name_.str();
        s.to_png(name);
    }

    return 0;
}
