
#ifndef SOLVER_ORIGAMI_HPP
#define SOLVER_ORIGAMI_HPP

#include <zebra/geometry.hpp>
#include <zebra/log.hpp>
#include <zebra/solution.hpp>

#include <CGAL/Boolean_set_operations_2.h>

#include <unordered_set>

namespace zebra
{
class origami
{
public:
    solution sol = solution::unit_square();

    std::vector<origami> move_to(point p) const
    {
        logging::debug() << "MOVE_TO (" << p << ")[[[[\n" << sol << "\n]]]]] MOVE_TO";
        std::vector<origami> results;
        results.reserve(sol.destination_positions().size() * 2);
        for (auto dp : sol.destination_positions())
        {
            auto moved = *this;
            moved.sol.transform(translation(p - dp));
            logging::trace() << "MOVED (" << dp << ")[[[[\n" << moved.sol << "\n]]]]] MOVED";

            results.push_back(moved);
        }
        return results;
    }

    std::vector<origami> folds(const line& l) const
    {
        std::vector<origami> results;
        std::unordered_set<facet_id> fold_facets;

        for (vertex_id vid = 0; vid < sol.vertex_size(); vid++)
        {
            if (l.has_on_negative_side(sol.destination_position(vid)))
            {
                for (auto fid : sol.facets_by_vertex(vid))
                {
                    fold_facets.insert(fid);
                }
            }
        }
        while (!fold_facets.empty())
        {
            facet_id fid = *fold_facets.begin();
            auto folded = *this;
            auto folded_facets_and_vertices = folded.sol.fold(fid, l);
            const auto& folded_facets = folded_facets_and_vertices.first;
            for (facet_id folded_facet_id = 0; folded_facet_id < folded_facets.size();
                 folded_facet_id++)
            {
                if (folded_facets.contains(folded_facet_id))
                {
                    fold_facets.erase(folded_facet_id);
                }
            }
        }
        logging::debug() << "Found " << results.size() << " separate folds.";
        return results;
    }
};
}

#endif // SOLVER_ORIGAMI_HPP
