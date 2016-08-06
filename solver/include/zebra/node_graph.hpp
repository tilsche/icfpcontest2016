#ifndef __ZEBRA_NODE_GRAPH_HPP__
#define __ZEBRA_NODE_GRAPH_HPP__

#include <vector>
#include <algorithm>
#include <set>

#include "geometry.hpp"
#include "task.hpp"

namespace zebra
{
    struct node_graph
    {
        node_graph()
        {}

        node_graph(task& t)
        {
            for (const auto& edge : t.skel.edges)
            {
                if (m_graph.count(edge.source()) == 0) {
                    m_graph[edge.source()] = std::set<point>{edge.target()};
                }
                else {
                    m_graph[edge.source()].insert(edge.target());
                }

                if (m_graph.count(edge.target()) == 0) {
                    m_graph[edge.target()] = std::set<point>{edge.source()};
                }
                else {
                    m_graph[edge.target()].insert(edge.source());
                }
            }
            
            for(const auto& edge : t.skel.edges)
            {
                for(const auto& map_entry : m_graph)
                {
                    if(edge.has_on(map_entry.first)
                    && edge.source() != map_entry.first
                    && edge.target() != map_entry.first)
                    {
                        m_graph[map_entry.first].insert(edge.source());
                        m_graph[map_entry.first].insert(edge.target());
                        m_graph[edge.source()].insert(map_entry.first);
                        m_graph[edge.target()].insert(map_entry.first);
                        m_graph[edge.source()].erase(edge.target());
                        m_graph[edge.target()].erase(edge.source());
                    }
                }
            }
            
            for(const auto& poly : t.sil.polygons)
            {
                std::vector<point> polygon_i;
                std::for_each(poly.vertices_begin(), poly.vertices_end(), [&](point p){
                    polygon_i.push_back(p);
                    });
                m_polys.push_back(polygon_i);
            }
        }

        std::set<point>& operator[](const point& p)
        {
            return m_graph[p];
        }

        size_t size()
        {
            return m_graph.size();
        }

        std::vector<std::vector<point>> m_polys;
        std::map<point, std::set<point>> m_graph;
    };

    // JUST FOR DEBUGGING
    void print_node_graph(node_graph& ng)
    {
        for (const auto& kvp : ng.m_graph)
        {
            std::cout << "Key: " << point_to_string(kvp.first) << "\n\t";
            for (const auto& pnt : kvp.second) {
                std::cout << "Value: " << point_to_string(pnt) << "\n\t";
            }
            std::cout << "\n\n";
        }
    }
} /* closing namespace zebra */

#endif /* __ZEBRA_NODE_GRAPH_HPP__ */
