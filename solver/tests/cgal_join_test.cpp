#include <CGAL/Gmpz.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Boolean_set_operations_2.h>

#include <vector>

using kernel = CGAL::Simple_cartesian<CGAL::Gmpq>;
using point = CGAL::Point_2<kernel>;
using polygon = CGAL::Polygon_2<kernel>;
using polygon_with_holes = CGAL::Polygon_with_holes_2<kernel>;
using polygon_set = CGAL::Polygon_set_2<kernel>;

std::vector<polygon> polys;

void add(const std::vector<point>& vs)
{
    polygon p(vs.begin(), vs.end());
    assert(p.is_simple());
    assert(p.is_counterclockwise_oriented());
    assert(p.is_convex());
    polys.emplace_back(p);
}

int main()
{
    add({ point(CGAL::Gmpq("32269/2187934"), CGAL::Gmpq("3904671/48134548")),point(CGAL::Gmpq("52114015/227230296"), CGAL::Gmpq("-56907582/66275503")),point(CGAL::Gmpq("199378943155307/439658962005424"), CGAL::Gmpq("-524116104937731/549573702506780")),point(CGAL::Gmpq("311258412215/588585285232"), CGAL::Gmpq("-570907098983/735731606540")),point(CGAL::Gmpq("43501/227626"), CGAL::Gmpq("277381/1138130")) });
    add({ point(CGAL::Gmpq("137/504"), CGAL::Gmpq("0/1")),point(CGAL::Gmpq("43501/227626"), CGAL::Gmpq("277381/1138130")),point(CGAL::Gmpq("32269/2187934"), CGAL::Gmpq("3904671/48134548")) });
    add({ point(CGAL::Gmpq("201195123/914722480"), CGAL::Gmpq("25583467/163343300")),point(CGAL::Gmpq("205941/2620880"), CGAL::Gmpq("-33127/65522")),point(CGAL::Gmpq("13006547/27868104"), CGAL::Gmpq("-1594346/2709399")) });
    add({ point(CGAL::Gmpq("43501/227626"), CGAL::Gmpq("277381/1138130")),point(CGAL::Gmpq("-302782477/1808931376"), CGAL::Gmpq("-831892319/2261164220")),point(CGAL::Gmpq("311258412215/588585285232"), CGAL::Gmpq("-570907098983/735731606540")) });
    add({ point(CGAL::Gmpq("19/80"), CGAL::Gmpq("2249/21700")),point(CGAL::Gmpq("4268587/27221488"), CGAL::Gmpq("5196187727/24499339200")),point(CGAL::Gmpq("32269/2187934"), CGAL::Gmpq("3904671/48134548")),point(CGAL::Gmpq("137/504"), CGAL::Gmpq("0/1")) });
    add({ point(CGAL::Gmpq("32269/2187934"), CGAL::Gmpq("3904671/48134548")),point(CGAL::Gmpq("52114015/227230296"), CGAL::Gmpq("-56907582/66275503")),point(CGAL::Gmpq("13006547/27868104"), CGAL::Gmpq("-1594346/2709399")) });
    add({ point(CGAL::Gmpq("32269/2187934"), CGAL::Gmpq("3904671/48134548")),point(CGAL::Gmpq("52114015/227230296"), CGAL::Gmpq("-56907582/66275503")),point(CGAL::Gmpq("199378943155307/439658962005424"), CGAL::Gmpq("-524116104937731/549573702506780")),point(CGAL::Gmpq("311258412215/588585285232"), CGAL::Gmpq("-570907098983/735731606540")),point(CGAL::Gmpq("43501/227626"), CGAL::Gmpq("277381/1138130")) });

    std::vector<polygon_with_holes> union_polys;
    CGAL::join(polys.begin(), polys.end(), std::back_inserter(union_polys));
}
