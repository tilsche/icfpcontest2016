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
        using iterator = std::map<upoint, std::set<upoint>>::iterator;

        node_graph()
        {}

        node_graph(const task& t) : m_polys(t.sil.polygons())
        {
            std::map<point, upoint> pup;

            for (const auto& edge : t.skel.edges)
            {
                if (pup.count(edge.source()) == 0) {
                    pup[edge.source()] = upoint(edge.source());
                }
                if (pup.count(edge.target()) == 0) {
                    pup[edge.target()] = upoint(edge.target());
                }
            }

            for (const auto& edge : t.skel.edges)
            {
                m_graph[pup[edge.source()]].insert(pup[edge.target()]);
                m_graph[pup[edge.target()]].insert(pup[edge.source()]);
            }

            for(const auto& edge : t.skel.edges)
            {
                for(const auto& map_entry : m_graph)
                {
                    if(edge.has_on(map_entry.first)
                    && edge.source() != map_entry.first
                    && edge.target() != map_entry.first)
                    {
                        m_graph[map_entry.first].insert(pup[edge.source()]);
                        m_graph[map_entry.first].insert(pup[edge.target()]);
                        m_graph[pup[edge.source()]].insert(map_entry.first);
                        m_graph[pup[edge.target()]].insert(map_entry.first);
                        m_graph[pup[edge.source()]].erase(pup[edge.target()]);
                        m_graph[pup[edge.target()]].erase(pup[edge.source()]);
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

        std::set<upoint>& operator[](const upoint& p)
        {
            return m_graph[p];
        }

        bool operator<(const node_graph& other) const
        {
            return size() < other.size();
        }

        bool operator==(node_graph other) const
        {
            return m_graph == other.m_graph;
        }

        bool operator!=(const node_graph& other) const
        {
            return !(*this == other);
        }

        size_t size() const
        {
            return m_graph.size();
        }

        std::vector<polygon> m_polys;
        std::map<upoint, std::set<upoint>> m_graph;
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
