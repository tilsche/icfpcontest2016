#ifndef ZEBRA_SOLVER_BACKWARD_CONSTRAINTS_HPP
#define ZEBRA_SOLVER_BACKWARD_CONSTRAINTS_HPP
#include <cmath>

#include <zebra/node_graph.hpp>
#include <zebra/geometry.hpp>
#include <CGAL/squared_distance_2.h>

namespace zebra{

    class BackwardConstraints{

        public:

        static bool valid_length(point s, point e)
        {
            double distance = CGAL::to_double(CGAL::squared_distance(s, e));
            distance = sqrt(distance);
            return distance <= sqrt(2);
        }

        static bool is_standard_square(node_graph & ng)
        {
            std::vector<point> std_square(4);

            std_square.push_back(point(0,0));
            std_square.push_back(point(1,0));
            std_square.push_back(point(0,1));
            std_square.push_back(point(1,1));

            if(ng.m_graph.size() != 4)
            {
                return false;
            }
            for(auto & elem: ng.m_graph)
            {
                auto pos = std::find(std_square.begin(), std_square.end(), elem.first);
                if(pos == std_square.end())
                    return false;
            }
            return true;
        }

    };

};

#endif /* ZEBRA_SOLVER_BACKWARD_CONSTRAINTS_HPP */
