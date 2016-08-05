
#ifndef SOLVER_SOLUTION_HPP
#define SOLVER_SOLUTION_HPP

#include "geometry.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

namespace zebra
{

struct solution
{
    polygon source_positions;
    std::vector<std::vector<int>> facets;
    polygon destination_positions;

    std::vector<polygon> source_facets() const
    {
        std::vector<polygon> sf(facets.size());
        int i = 0;
        for (const auto& facet : facets)
        {
            for (int vid = 0; vid < facet.size(); vid++)
            {
                sf[i].push_back(source_positions[vid]);
            }
            i++;
        }
        return sf;
    }

    std::vector<polygon> destination_facets() const
    {
        std::vector<polygon> sf(facets.size());
        int i = 0;
        for (const auto& facet : facets)
        {
            for (int vid = 0; vid < facet.size(); vid++)
            {
                sf[i].push_back(destination_positions[vid]);
            }
            i++;
        }
        return sf;
    }

    bool is_valid() const
    {
        // All source vertices in 0,0 1,1
        // No duplicate source vertex
        std::set<point> source_set;
        for (auto vit = source_positions.vertices_begin(); vit != source_positions.vertices_end();
             vit++)
        {
            auto v = *vit;
            if (v.x() > 1 || v.x() < 0 || v.y() > 1 || v.y() < 0)
            {
                return false;
            }

            auto ret = source_set.insert(v);
            if (!ret.second)
            {
                return false;
            }
        }

        assert(source_positions.size() == destination_positions.size());

        auto sfs = source_facets();
        auto dfs = destination_facets();

        assert(dfs.size() == sfs.size());

        for (int i = 0; i < dfs.size(); i++)
        {
            auto& sf = sfs[i];
            auto& df = dfs[i];
            assert(sf.size() == sf.size());

            if (!sf.is_simple() || !df.is_simple())
            {
                return false;
            }

            for (int ei = 0; ei < sf.size(); ei++)
            {
                // TODO Check possible transformation!!!!
                auto source_edge = sf.edge(ei);
                auto destination_edge = df.edge(ei);

                if (source_edge.squared_length() == 0)
                {
                    return false;
                }
                if (source_edge.squared_length() != destination_edge.squared_length())
                {
                    return false;
                }
            }

            for (auto eit = source_positions.edges_begin(); eit != source_positions.edges_end();
                 eit++)
            {
                auto edge = *eit;
                if (edge.source() == edge.target())
                {
                    return false;
                }
            }
            return true;
        }
    }
};

inline std::ostream& operator<<(std::ostream& os, const solution& s)
{
    assert(s.source_positions.size() == s.destination_positions.size());
    os << s.source_positions.size() << "\n";
    for (auto it = s.source_positions.vertices_begin(); it != s.source_positions.vertices_end();
         it++)
    {
        const auto& vertex = *it;
        os << point_to_string(vertex) << "\n";
    }

    os << s.facets.size() << "\n";
    for (const auto& facet : s.facets)
    {
        for (auto vertex_id : facet)
        {
            os << vertex_id << " ";
        }
        os << "\n";
    }

    for (auto it = s.destination_positions.vertices_begin();
         it != s.destination_positions.vertices_end(); it++)
    {
        const auto& vertex = *it;
        os << point_to_string(vertex) << "\n";
    }
    return os;
}
}

#endif // SOLVER_SOLUTION_HPP
