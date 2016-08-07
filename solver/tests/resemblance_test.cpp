#include <zebra/log.hpp>

using namespace std;
using namespace zebra;

void test0();
void test1();

int main()
{
    test1();
    return 0;
}

void test1()
{
    // polygon p1;
    // p1.push_back({0,0});
    // p1.push_back({0,1});
    // p1.push_back({1,1});
    // p1.push_back({1,0});

    // polygon p2;
    // p2.push_back({0               , 0});
    // p2.push_back({0               , CGAL::Gmpq(1, 2)});
    // p2.push_back({CGAL::Gmpq(1, 2), CGAL::Gmpq(1, 2)});
    // p2.push_back({CGAL::Gmpq(1, 2), 0});

    // polygon p3;
    // p3.push_back({CGAL::Gmpq(1, 2), CGAL::Gmpq(1, 2)});
    // p3.push_back({1               , CGAL::Gmpq(1, 2)});
    // p3.push_back({1               , 1});
    // p3.push_back({1               , CGAL::Gmpq(1, 2)});

    // polygon p4;
    // p4.push_back({CGAL::Gmpq(1, 4), CGAL::Gmpq(1, 4)});
    // p4.push_back({CGAL::Gmpq(3, 4), CGAL::Gmpq(1, 4)});
    // p4.push_back({CGAL::Gmpq(3, 4), CGAL::Gmpq(3, 4)});
    // p4.push_back({CGAL::Gmpq(1, 4), CGAL::Gmpq(3, 4)});

    // silhouette s1;
    // s1.polygons.push_back(p1);

    // silhouette s2;
    // s2.polygons.push_back(p2);
    // s2.polygons.push_back(p3);

    // silhouette s3;
    // s3.polygons.push_back(p1);
    // s3.polygons.push_back(p4);

    // solution s;
    // s.source_positions = { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 } };
    // s.facets_ = { facet{.vertex_ids_ = { 0, 1, 2, 3 } } };
    // s.destination_positions = { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 } };

    // cerr << "s1: " << 1    << ", " << s.resemblance(s1) << endl;
    // cerr << "s2: " << 0.5  << ", " << s.resemblance(s2) << endl;
    // cerr << "s3: " << 0.75 << ", " << s.resemblance(s3) << endl;
}
