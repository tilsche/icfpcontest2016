#ifndef __ZEBRA_BACKWARD_SOLVE_HPP__
#define __ZEBRA_BACKWARD_SOLVE_HPP__

#include <memory>
#include <utility>
#include <vector>

#include "geometry.hpp"
#include "task.hpp"

#include <CGAL/Point_2.h>
#include <CGAL/Direction_2.h>


namespace zebra {

    template<typename T> struct bw_segment;

    template<typename T>
    struct bw_point
    {
        bw_point(T x, T y): point{x, y}
        {
        }

        void add_segment(const bw_segment<T>& ls)
        {
            segments.push_back(ls);
        }

        std::pair<T, T> point;
        std::vector<bw_segment<T>> segments;
    };

    template<typename T>
    struct bw_segment
    {
        bw_segment(bw_point<T> *p1, bw_point<T> *p2) : start{p1}, end{p2}
        {
            auto dx = end.point.first - start.point.first;
            auto dy = end.point.second - start.point.second;
            normal = std::make_pair<T, T>(-dy, dx);
        }

        std::unique_ptr<bw_point<T>> start;
        std::unique_ptr<bw_point<T>> end;
        std::pair<T, T> normal;

    };


    void solve_backward(task& t)
    {
        std::vector<std::pair<point, point>> point_lst;
        for (const auto& edge : t.skel.edges)
        {
            point_lst.emplace_back(edge.target(), edge.source());
            //auto direction = edge.direction();
            //auto normal = Kernel::Direction_2(-direction.dy(), direction.dx());
        }


    }


} /* closing namespace zebra */

#endif /* __ZEBRA_BACKWARD_SOLVE_HPP__ */
