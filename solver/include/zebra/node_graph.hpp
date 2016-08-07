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
        using iterator = std::map<point, std::set<point>>::iterator;

        node_graph()
        {}

        node_graph(const task& t)
        : m_polys(t.sil.polygons())
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
        }

        iterator begin()
        {
            return m_graph.begin();
        }

        iterator end()
        {
            return m_graph.end();
        }

        std::set<point>& operator[](const point& p)
        {
            return m_graph[p];
        }

        size_t size()
        {
            return m_graph.size();
        }

        std::vector<polygon> m_polys;
        std::map<point, std::set<point>> m_graph;
    };

    struct s2dmap
    {
        using map_iterator = std::map<point, std::set<point>>::iterator;

        // maps destination points to source points
        s2dmap()
        {
        }

        s2dmap(const node_graph& ng)
        {
            for (const auto& kvp : ng.m_graph)
            {
                m_map[kvp.first] = std::set<point>{kvp.first};
            }

        }

        std::vector<point> destination_points() const
        {
            std::vector<point> ret;
            for (const auto& kvp : m_map)
            {
                ret.push_back(kvp.first);
            }
            return ret;
        }

        std::set<point>& sources_points_of_destination(const point& dest)
        {
            return m_map[dest];
        }

        std::set<point>& operator[](const point& p)
        {
            return sources_points_of_destination(p);
        }

        map_iterator begin()
        {
            return m_map.begin();

        }

        map_iterator end()
        {
            return m_map.end();
        }

        private:
        std::map<point, std::set<point>> m_map;
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
