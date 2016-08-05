
#ifndef SOLVER_ORIGAMI_HPP
#define SOLVER_ORIGAMI_HPP

#include <zebra/geometry.hpp>

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
            transofrmation move(CGAL::TRANSLATION, edge.source());
            auto moved = move(*this);
            results.push_back(moved);
        }
        return results;
    }

    size_t size() const
    {
        return poly.size();
    }

    polygon poly = { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 } };
};
}

#endif // SOLVER_ORIGAMI_HPP
