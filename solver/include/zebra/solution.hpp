
#ifndef SOLVER_SOLUTION_HPP
#define SOLVER_SOLUTION_HPP

#include "geometry.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

namespace zebra
{

struct solution
{
    polygon source_positions;
    std::vector<std::vector<int>> facets;
    polygon destination_positions;
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

    os << s.facets.size();
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
