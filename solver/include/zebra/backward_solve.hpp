#ifndef __ZEBRA_BACKWARD_SOLVE_HPP__
#define __ZEBRA_BACKWARD_SOLVE_HPP__

#include <memory>
#include <utility>
#include <vector>
#include <algorithm>
#include <set>

#include "geometry.hpp"
#include "task.hpp"

#include <CGAL/Point_2.h>
#include <CGAL/Direction_2.h>


namespace zebra {


    struct node_graph {

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
        }

        std::map<point, std::set<point>> m_graph;
    };


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




    //template<typename T> struct bw_segment;

    //template<typename T>
    //struct bw_point
    //{
        //bw_point(T x, T y): point{x, y}
        //{
        //}

        //~bw_point() {}

        //void add_segment(std::shared_ptr<bw_segment<T>> ls)
        //{
            //segments.push_back(ls);
        //}

        //std::pair<T, T> point;
        //std::vector<std::shared_ptr<bw_segment<T>>> segments;
    //};

    //template<typename T>
    //struct bw_segment
    //{
        //bw_segment(std::shared_ptr<bw_point<T>> p1, std::shared_ptr<bw_point<T>> p2) : start{p1}, end{p2}
        //{
            //auto dx = end.get()->point.first - start.get()->point.first;
            //auto dy = (end.get()->point.second - start.get()->point.second) * -1;
            //normal = std::make_pair<T, T>(dy, dx);
        //}

        //std::shared_ptr<bw_point<T>> start;
        //std::shared_ptr<bw_point<T>> end;
        //std::pair<T, T> normal;

    //};


    //void solve_backward(task& t)
    //{
        //std::vector<std::pair<point, point>> point_lst;
        //for (const auto& edge : t.skel.edges)
        //{
            //point_lst.emplace_back(edge.target(), edge.source());
            ////auto direction = edge.direction();
            ////auto normal = Kernel::Direction_2(-direction.dy(), direction.dx());
        //}

        //std::vector<point> pvec; // unique point list
        //std::vector<std::shared_ptr<bw_point<CGAL::Gmpq>>> bwvec; // unique bwpoint list

        //for (const auto& elem : point_lst)
        //{
            //auto it = std::find(pvec.begin(), pvec.end(), elem.first);
            //if (it == pvec.end()) {
                //pvec.push_back(elem.first);
                //bwvec.emplace_back(std::make_shared<bw_point<CGAL::Gmpq>(elem.first.x(), elem.first.y()));
            //}

            //it = std::find(pvec.begin(), pvec.end(), elem.second);
            //if (it == pvec.end()) {
                //pvec.push_back(elem.second);
                //bwvec.push_back(std::make_shared<bw_point<CGAL::Gmpq>(elem.second.x(), elem.second.y()));
            //}
        //}

        //std::vector<std::shared_ptr<bw_segment<CGAL::Gmpq>>> seglst;
        //for (const auto& elem : point_lst)
        //{
            //int first = 0;
            //int second = 0;
            //for (int i = 0; i < pvec.size(); ++i)
            //{
                //if (pvec[i] == elem.first) {
                    //first = i;
                //}

                //if (pvec[i] == elem.second) {
                    //second = i;
                //}
            //}

            //seglst.push_back(std::make_shared<bw_segment<CGAL::Gmpq>(bwvec[first], bwvec[second]));
            //bwvec[first]->add_segment(seglst.back());
            //bwvec[second]->add_segment(seglst.back());
        //}



    //}


} /* closing namespace zebra */

#endif /* __ZEBRA_BACKWARD_SOLVE_HPP__ */
