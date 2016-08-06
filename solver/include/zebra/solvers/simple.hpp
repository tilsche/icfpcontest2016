
#ifndef SOLVER_SIMPLE_HPP
#define SOLVER_SIMPLE_HPP

#include <zebra/solver.hpp>

#include <zebra/log.hpp>
#include <zebra/origami.hpp>

namespace zebra
{
class simple : public solver
{
public:
    solution operator()(task t) override
    {
        logging::info() << "Simple solver starting..";
        solution s;
        s.source_positions = make_positions_1();
        s.facets.push_back(facet{.vertex_ids = { 0, 1, 2, 3 } });

        assert(t.sil.polygons.size() == 1);
        polygon target = t.sil.polygons[0];
        assert(target.is_counterclockwise_oriented());

        origami ori;
        double best_r = 0;
        origami best_ori;
        for (auto vx_it = target.vertices_begin(); vx_it != target.vertices_end(); vx_it++)
        {
            for (auto o2 : ori.move_to(*vx_it))
            {
                auto new_r = o2.resemblance(target);
                if (new_r > best_r)
                {
                    best_r = new_r;
                    best_ori = o2;
                }
            }
        }
        auto vector = best_ori.poly.vertex(0) - ori.poly.vertex(0);
        logging::info() << "Simple found best resemblence " << best_r
                        << " vector: " << gmpq_to_string(vector.hx()) << ", "
                        << gmpq_to_string(vector.hy());

        transformation move(CGAL::TRANSLATION, vector);
        s.destination_positions = make_positions_1();
        for (auto& destination_position : s.destination_positions)
        {
            destination_position = move(destination_position);
        }
        return s;
    }
};
}
#endif // SOLVER_SIMPLE_HPP
