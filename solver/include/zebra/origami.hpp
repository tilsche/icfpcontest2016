
#ifndef SOLVER_ORIGAMI_HPP
#define SOLVER_ORIGAMI_HPP

#include <zebra/geometry.hpp>

#include <CGAL/Boolean_set_operations_2.h>

namespace zebra
{
class origami
{
public:
    void fold(line_segment seg)
    {
        /*
        bool source_found = false, destination_found = false;
        polygon s2d_poly, d2s_poly;
        for (auto it = poly.edges_circulator();; it++)
        {
            const auto& edge = *it;
            if (!source_found)
            {
                if (edge.has_on(seg.source()))
                {
                    source_found = true;
                    s2d_poly.push_back(seg.source());
                    if (seg.source() != edge.dest())
                    {
                        s2d_poly.push_back(edge.dest());
                    }
                }
            }
            else if (!destination_found)
            {
                if (edge.has_on(seg.destination()))
                {
                    destination_found = true;
                    d2s_poly.push_back(seg.dest());
                    if (seg.dest() != edge.dest())
                    {
                        d2s_poly.push_back(edge.dest());
                    }
                }
                else
                {
                    s2d_poly.push_back(edge.dest());
                }
            }
            else
            {
                d2s_poly.push_back(edge.dest());
                if (edge.has_on(seg.source()))
                {
                    break;
                }
            }
        }
         */
    }

    std::vector<origami> move_to(line_segment seg) const
    {
        std::vector<origami> results;
        results.reserve(poly.size() * 2);
        for (auto edge_it = poly.edges_begin(); edge_it != poly.edges_end(); edge_it++)
        {
            const auto& edge = *edge_it;
            transofrmation move(CGAL::TRANSLATION, seg.source() - edge.source());
            auto moved = *this;
            moved.poly = CGAL::transform(move, moved.poly);
            results.push_back(moved);
        }
        return results;
    }

    double resemblance(polygon target) const
    {
        polygon_with_holes p_or;
        std::vector<polygon_with_holes> p_ands;
        CGAL::join(poly, target, p_or);
        CGAL::intersection(poly, target, std::back_inserter(p_ands));
        CGAL::Gmpq and_area = 0;
        for (const auto& holy : p_ands)
        {
            and_area += holy.outer_boundary().area();
        }
        auto or_area = p_or.outer_boundary().area();
        return gmpq_to_double(and_area / or_area);
    }

    size_t size() const
    {
        return poly.size();
    }

    polygon poly = make_polygon_1();
};
}

#endif // SOLVER_ORIGAMI_HPP
