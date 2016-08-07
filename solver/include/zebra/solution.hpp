// !!!!!!!!!!!!!!!!!!!!!!!!!!!
// ! NEGATIVE WILL BE FOLDED !
// ! NEGATIVE WILL BE FOLDED !
// !!!!!!!!!!!!!!!!!!!!!!!!!!!

#ifndef SOLVER_SOLUTION_HPP
#define SOLVER_SOLUTION_HPP

#include <zebra/facet.hpp>
#include <zebra/geometry.hpp>
#include <zebra/log.hpp>
#include <zebra/silhouette.hpp>

#include <CGAL/Boolean_set_operations_2.h>

#include <boost/algorithm/string/erase.hpp>

#include <bitset>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace zebra
{
template <class ID>
class id_set
{
public:
    id_set(ID size)
    {
        data.resize(size, false);
    }

    bool insert(ID id)
    {
        if (data[id])
        {
            return false;
        }
        data[id] = true;
        return true;
    }

    bool contains(ID id) const
    {
        return data[id];
    }

    ID size() const
    {
        return data.size();
    }

private:
    std::vector<bool> data;
};
using facet_id_set = id_set<facet_id>;
using vertex_id_set = id_set<vertex_id>;

class solution;

inline std::ostream& operator<<(std::ostream& os, const solution& s);

class solution
{
private:
    std::vector<point> source_positions_;
    std::vector<point> destination_positions_;
    std::map<point, vertex_id> source_positions_by_point_;
    std::vector<facet> facets_;
    std::vector<std::unordered_set<facet_id>> facets_by_vertex_;

public:
    const std::vector<point>& source_positions() const
    {
        return source_positions_;
    }

    const point& source_position(vertex_id vid) const
    {
        return source_positions_[vid];
    }

    const std::vector<point>& destination_positions() const
    {
        return destination_positions_;
    }

    const point& destination_position(vertex_id vid) const
    {
        return destination_positions_[vid];
    }

    const std::unordered_set<facet_id>& facets_by_vertex(vertex_id vid) const
    {
        return facets_by_vertex_[vid];
    }

    size_t vertex_size() const
    {
        assert(source_positions().size() == destination_positions().size());
        assert(source_positions().size() == facets_by_vertex_.size());
        return destination_positions().size();
    }

    const std::vector<facet>& facets() const
    {
        return facets_;
    }

    size_t facet_size() const
    {
        return facets_.size();
    }

    const facet& get_facet(facet_id id) const
    {
        return facets_[id];
    }

    facet& get_facet(facet_id id)
    {
        return facets_[id];
    }

public:
    // TODO universal reference
    facet_id facet_add(facet f)
    {
        facet_id new_id = facet_size();
        facet_register(new_id, f);
        facets_.emplace_back(std::move(f));
        return new_id;
    }

protected:
    void facet_register(facet_id id)
    {
        facet_register(id, facets_[id]);
    }

    void facet_register(facet_id id, const facet& f)
    {
        for (auto v : f.vertex_ids())
        {
            auto r = facets_by_vertex_[v].insert(id);
            assert(r.second);
            (void)r;
        }
#ifndef NDEBUG
        facet_poly(f);
#endif
    }

    void facet_unregister(facet_id id)
    {
        for (auto v : facets_[id].vertex_ids())
        {
            auto removed = facets_by_vertex_[v].erase(id);
            assert(removed == 1);
            (void)removed;
        }
    }

    void facet_swap(facet_id old_id, facet f_new)
    {
        facet_unregister(old_id);
        facets_[old_id] = std::move(f_new);
        facet_register(old_id);
    }

public:
    vertex_id vertex_add(point source_destination_point)
    {
        return vertex_add(source_destination_point, source_destination_point);
    }

    vertex_id vertex_add(point source_point, point destination_point)
    {
        auto new_idx = vertex_size();

        auto try_insert = source_positions_by_point_.emplace(std::make_pair(source_point, new_idx));
        if (try_insert.second)
        {
            destination_positions_.push_back(destination_point);
            source_positions_.push_back(source_point);
            facets_by_vertex_.resize(new_idx + 1);
            (void)(vertex_size());
            return new_idx;
        }
        else
        {
            auto old_id = try_insert.first->second;
            assert(destination_positions_[old_id] == destination_point);
            return old_id;
        }
    }

    std::vector<polygon> source_facets() const
    {
        std::vector<polygon> sf(facet_size());
        size_t i = 0;
        for (const auto& f : facets_)
        {
            for (size_t vid = 0; vid < f.size(); vid++)
            {
                sf[i].push_back(source_positions_[vid]);
            }
            i++;
        }
        return sf;
    }

    std::vector<polygon> destination_facets() const
    {
        std::vector<polygon> sf(facet_size());
        size_t i = 0;
        for (const auto& f : facets_)
        {
            for (size_t vid = 0; vid < f.size(); vid++)
            {
                sf[i].push_back(destination_positions_[vid]);
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
        for (const auto& v : source_positions_)
        {
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

        auto sfs = source_facets();
        auto dfs = destination_facets();

        assert(dfs.size() == sfs.size());

        for (size_t i = 0; i < dfs.size(); i++)
        {
            auto& sf = sfs[i];
            auto& df = dfs[i];
            assert(sf.size() == sf.size());

            if (!sf.is_simple() || !df.is_simple())
            {
                return false;
            }

            for (size_t ei = 0; ei < sf.size(); ei++)
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
        }
        return true;
    }

    line_segment destination_segment(const facet& f, size_t segment_id) const
    {
        assert(segment_id < f.size());
        auto s = line_segment(destination_positions_[f[segment_id]],
                              destination_positions_[f[segment_id + 1]]);
        if (s.source() == s.target())
        {
            logging::error() << "Invalid destination segment: " << s;
        }
        return s;
    }

    line_segment source_segment(const facet& f, size_t segment_id) const
    {
        assert(segment_id < f.size());
        auto s =
            line_segment(source_positions_[f[segment_id]], source_positions_[f[segment_id + 1]]);
        if (s.source() == s.target())
        {
            logging::error() << "Invalid destination segment: " << s;
        }
        return s;
    }

    void facet_mirror(facet_id id, line fold_line)
    {
        // DOES NOT YET MIRROR THE POINTS
        auto mirror = reflection(fold_line);
        get_facet(id).transform(mirror);
    }

    void facet_fold(facet_id id, line fold_line)
    {
        size_t positive_points = 0;
        size_t negative_points = 0;
        for (auto vertex_id : get_facet(id).vertex_ids())
        {
            const auto& vertex = destination_positions_[vertex_id];
            switch (fold_line.oriented_side(vertex))
            {
            case CGAL::ON_POSITIVE_SIDE:
                positive_points++;
                break;
            case CGAL::ON_NEGATIVE_SIDE:
                negative_points++;
                break;
            case CGAL::ON_ORIENTED_BOUNDARY:
                break;
            }
        }
        // we really need to split
        if (positive_points > 0 && negative_points > 0)
        {
            auto new_facet_id = facet_split(id, fold_line);
            facet_mirror(new_facet_id, fold_line);
        }
        else if (negative_points > 0)
        {
            facet_mirror(id, fold_line);
        }
    }

    facet_id facet_split(facet_id old_id, line fold_line)
    {
        auto& old_facet = get_facet(old_id);
        facet facet_positive(old_facet.transform());
        facet facet_negative(old_facet.transform());
        for (size_t segment_id = 0; segment_id < old_facet.size(); segment_id++)
        {
            auto vertex_id = old_facet[segment_id];
            auto segment = destination_segment(old_facet, segment_id);
            auto vertex = segment.source();

            // Not so important check
            assert(vertex == destination_positions_[vertex_id]);

            logging::debug() << "vertex " << vertex_id << "@" << vertex << ", segment " << segment;
            switch (fold_line.oriented_side(vertex))
            {
            case CGAL::ON_POSITIVE_SIDE:
                logging::debug() << " POSITIVE";
                facet_positive.vertex_ids_.push_back(vertex_id);
                break;
            case CGAL::ON_NEGATIVE_SIDE:
                logging::debug() << "NEGATVE";
                facet_negative.vertex_ids_.push_back(vertex_id);
                break;
            case CGAL::ON_ORIENTED_BOUNDARY:
                logging::debug() << "BOUNDARY";
                facet_positive.vertex_ids_.push_back(vertex_id);
                facet_negative.vertex_ids_.push_back(vertex_id);
                break;
            }
            boost::optional<boost::variant<point, line_segment>> o =
                CGAL::intersection(segment, fold_line);

            if (o != boost::none)
            {
                if (o->which() == 0)
                {
                    // intersection_point is a point
                    auto intersection_point = boost::get<point>(*o);
                    if (intersection_point == segment.target() ||
                        intersection_point == segment.source())
                    {
                        logging::debug() << "Intersecting at line ";
                        // No need for new vertex - vertex id will be added in next iteration.
                    }
                    else
                    {
                        // need new vertex
                        auto source_point = old_facet.inverse()(intersection_point);
                        auto new_vertex_id = vertex_add(source_point, intersection_point);
                        logging::debug() << "new vertex " << new_vertex_id << "@"
                                         << intersection_point;
                        facet_positive.vertex_ids_.push_back(new_vertex_id);
                        facet_negative.vertex_ids_.push_back(new_vertex_id);
                    }
                }
                else
                {
                    // intersection is a line
                    logging::error() << "Intersection of segment " << segment << " on fold "
                                     << fold_line << " is a  line.";
                    assert(false);
                }
            }
        }
        facet_swap(old_id, facet_positive);
        return facet_add(facet_negative);
    }

private:
    std::vector<polygon> facet_polygons() const
    {
        std::vector<polygon> r;
        r.reserve(facet_size());
        for (const facet& f : facets_)
        {
            r.push_back(facet_poly(f));
        }
        return r;
    }

public:
    polygon facet_poly(const facet& f) const
    {
        polygon p;
        logging::trace() << "COMPUTING FACET_POLY: ";
        for (auto vid : f.vertex_ids())
        {
            logging::trace() << "adding " << vid << ": " << destination_position(vid);
            p.push_back(destination_position(vid));
        }
        if (!p.is_counterclockwise_oriented())
        {
            p = polygon();
            for (auto it = f.vertex_ids().rbegin(); it != f.vertex_ids().rend(); it++)
            {
                auto id = *it;
                p.push_back(destination_positions_[id]);
            }
            assert(p.is_counterclockwise_oriented());
        }
        logging::trace() << "FACET_POLY = " << p;
        assert(p.is_simple());
        assert(p.is_convex());
        assert(p.is_counterclockwise_oriented());
        return p;
    }

    void fold(const line& fold_line)
    {
        size_t old_facet_size = facet_size();
        for (auto facet_id = 0; facet_id < old_facet_size; facet_id++)
        {
            facet_fold(facet_id, fold_line);
        }
        auto mirror = reflection(fold_line);
        for (auto& destination_position : destination_positions_)
        {
            if (fold_line.has_on_negative_side(destination_position))
            {
                destination_position = mirror(destination_position);
            }
        }
        invalidate();
    }

    void fold(const line_segment& fold_segment)
    {
        fold(line(fold_segment.source(), fold_segment.target()));
    }

protected:
    template <typename T1, typename T2>
    void facet_connected(facet_id fid, const line& fold_line, T1& facets, T2& vertices)
    {
        // I will take care of this facet if not in already
        if (!facets.insert(fid))
        {
            // allready in;
            return;
        }
        // added to facets, so this will not be checked again
        for (auto vid : this->get_facet(fid).vertex_ids())
        {
            if (fold_line.has_on_negative_side(destination_position(vid)))
            {
                // i will take care of this vertex if not done already
                if (!vertices.insert(vid))
                {
                    // allready in
                    continue;
                }
                for (facet_id next_fid : facets_by_vertex_[vid])
                {
                    facet_connected(next_fid, fold_line, facets, vertices);
                }
            }
        }
    }

public:
    std::pair<facet_id_set, vertex_id_set> facet_negative_set(facet_id root_id,
                                                              const line& fold_line)
    {
        // Find all facets that are connected on the negative side of the fold_line
        facet_id_set facets(facet_size());
        vertex_id_set vertices(vertex_size());
        facet_connected(root_id, fold_line, facets, vertices);
        return { facets, vertices };
    }

    void fold(facet_id id, const line_segment& fold_segment)
    {
        fold(id, line(fold_segment.source(), fold_segment.target()));
    }

    std::pair<facet_id_set, vertex_id_set> fold(facet_id id, const line& fold_line)
    {
        auto facets_and_vertices = facet_negative_set(id, fold_line);
        const auto& facets = facets_and_vertices.first;
        const auto& vertices = facets_and_vertices.second;
        for (facet_id fid = 0; fid < facets.size(); fid++)
        {
            if (facets.contains(fid))
            {
                facet_fold(fid, fold_line);
            }
        }
        auto mirror = reflection(fold_line);
        for (vertex_id vid = 0; vid < vertices.size(); vid++)
        {
            if (vertices.contains(vid))
            {
                assert(fold_line.has_on_negative_side(destination_position(vid)));
                destination_positions_[vid] = mirror(destination_position(vid));
            }
        }
        invalidate();
        return facets_and_vertices;
    }

    void transform(transformation t)
    {
        for (auto& f : facets_)
        {
            f.transform(t);
        }
        for (auto& dp : destination_positions_)
        {
            dp = t(dp);
        }
        invalidate();
    }

    void rotate(const point& p, const line_segment& segment)
    {
        transform(translation(p) * rotation(segment) * translation(vector(-p.x(), -p.y())));
    }

    void translate(const point& p)
    {
        transform(translation(p));
    }

    std::size_t file_size() const
    {
        std::stringstream s;

        s << *this;

        auto txt = s.str();

        boost::erase_all(txt, " ");
        boost::erase_all(txt, "\n");

        return txt.size();
    }

    void to_png(const std::string& prefix) const
    {
        std::vector<std::string> unlink_files;
        // source points

        auto source_points_dat = prefix + "_source_points.dat";
        std::ofstream os(source_points_dat);
        unlink_files.push_back(source_points_dat);
        for (const auto& p : source_positions_)
        {
            os << gmpq_to_double(p.x()) << ' ' << gmpq_to_double(p.y()) << std::endl;
        }
        os.close();

        auto source_plot = prefix + "_source.plot";
        std::ofstream oss(source_plot);
        unlink_files.push_back(source_plot);
        oss << "set terminal pngcairo size 800,800" << std::endl;
        oss << "set output \"" << prefix << "_source.png\"" << std::endl;
        oss << "set size square" << std::endl;
        oss << "plot \"" << source_points_dat << "\" ps 3 pt 3, \\" << std::endl;

        // source facets_

        for (int i = 0; i < facet_size(); i += 1)
        {
            auto vertices = facets_[i].vertex_ids();
            vertices.push_back(vertices[0]);

            std::stringstream facet_dats;
            facet_dats << prefix << "_source_facet_" << i << ".dat";
            auto facet_dat = facet_dats.str();

            unlink_files.push_back(facet_dat);

            std::ofstream of(facet_dat);
            for (int j = 0; j < vertices.size(); j += 1)
            {
                auto p = source_positions_[vertices[j]];
                of << gmpq_to_double(p.x()) << ' ' << gmpq_to_double(p.y()) << std::endl;
            }
            of.close();

            oss << "\"" << facet_dat << "\" with lines lw 3";

            if (i < facet_size() - 1)
            {
                oss << ", \\" << std::endl;
            }
            else
            {
                oss << std::endl;
            }
        }

        oss.close();

        system(("gnuplot " + source_plot).c_str());

        // destination plot

        auto destination_points_dat = prefix + "_target.dat";
        std::ofstream od(destination_points_dat);
        unlink_files.push_back(destination_points_dat);
        for (const auto& p : destination_positions_)
        {
            od << gmpq_to_double(p.x()) << ' ' << gmpq_to_double(p.y()) << std::endl;
        }
        od.close();

        auto destination_plot = prefix + "_target.plot";
        std::ofstream odd(destination_plot);
        unlink_files.push_back(destination_plot);
        odd << "set terminal pngcairo size 800,800" << std::endl;
        odd << "set output \"" << prefix << "_target.png\"" << std::endl;
        oss << "set size square" << std::endl;
        odd << "plot \"" << destination_points_dat << "\" ps 3 pt 3, \\" << std::endl;

        // destination facets_

        for (int i = 0; i < facet_size(); i += 1)
        {
            auto vertices = facets_[i].vertex_ids();
            vertices.push_back(vertices[0]);

            std::stringstream facet_dats;
            facet_dats << prefix << "_destination_facet_" << i << ".dat";
            auto facet_dat = facet_dats.str();

            unlink_files.push_back(facet_dat);

            std::ofstream of(facet_dat);
            for (int j = 0; j < vertices.size(); j += 1)
            {
                auto p = destination_positions_[vertices[j]];
                of << gmpq_to_double(p.x()) << ' ' << gmpq_to_double(p.y()) << std::endl;
            }
            of.close();

            odd << "\"" << facet_dat << "\" with lines lw 3";

            if (i < facet_size() - 1)
            {
                odd << ", \\" << std::endl;
            }
            else
            {
                odd << std::endl;
            }
        }

        odd.close();

        system(("gnuplot " + destination_plot).c_str());

        for (const auto& f : unlink_files)
        {
            unlink(f.c_str());
        }
    }

    static solution unit_square()
    {
        solution ret;
        ret.vertex_add(point{ 0, 0 });
        ret.vertex_add(point{ 1, 0 });
        ret.vertex_add(point{ 1, 1 });
        ret.vertex_add(point{ 0, 1 });
        ret.facet_add(facet::unit_facet());
        return ret;
    }

    template <class POLY>
    double resemblance(POLY target) const
    {
        logging::trace() << "calculating resemblence with " << target;
        logging::trace() << "solution: " << *this;
        polygon_with_holes p_or;
        polygon_with_holes solution_poly = poly();
        logging::trace() << "solution_poly: " << solution_poly;
        std::vector<polygon_with_holes> p_ands;
        bool overlap = CGAL::join(solution_poly, target, p_or);
        if (!overlap)
        {
            logging::trace() << "NO overlap";
            return 0.0;
        }
        CGAL::intersection(solution_poly, target, std::back_inserter(p_ands));
        CGAL::Gmpq and_area = 0;
        for (const auto& holy : p_ands)
        {
            and_area += holy_area(holy);
        }
        auto or_area = holy_area(p_or);
        auto gmpr = and_area / or_area;
        double r = gmpq_to_double(gmpr);
        logging::trace() << "resmblance " << r << " = " << gmpr;
        return r;
    }

private:
    void invalidate()
    {
    }

public:
    polygon_with_holes poly() const
    {
        logging::trace() << "Generating polygon for SOLUTION[[[[\n" << *this << "SOLUTION]]]]\n";
        polygon_set ps;
        for (const facet& f : facets_)
        {
            ps.join(facet_poly(f));
        }
        assert(ps.number_of_polygons_with_holes() == 1);
        std::vector<polygon_with_holes> union_polys;
        ps.polygons_with_holes(std::back_inserter(union_polys));
        assert(union_polys.size() == 1);
        return union_polys[0];
    }

    polygon hull() const
    {
        return poly().outer_boundary();
    }

    static solution from_file(const std::string filename)
    {
        std::ifstream in(filename);
        assert(in);

        std::string s;

        getline(in, s);
        int source_points_count = std::stoi(s);

        std::vector<point> sps, dps;
        for (int i = 0; i < source_points_count; i += 1)
        {
            s = "";
            getline(in, s);
            sps.push_back(point_from_string(s));
        }

        s = "";
        getline(in, s);
        int facets_count = std::stoi(s);
        std::vector<facet> facets;
        for (int i = 0; i < facets_count; i += 1)
        {
            getline(in, s, ' ');
            int vertex_count = std::stoi(s);

            std::vector<vertex_id> vids;
            for (int j = 0; j < vertex_count; j += 1)
            {
                s = "";
                if (j < vertex_count - 1)
                {
                    getline(in, s, ' ');
                }
                else
                {
                    getline(in, s);
                }
                int vertex = std::stoi(s);
                vids.push_back(vertex);
            }

            facets.emplace_back(vids);
        }

        for (int i = 0; i < source_points_count; i += 1)
        {
            s = "";
            getline(in, s);
            dps.push_back(point_from_string(s));
        }

        solution ret;

        for (vertex_id vid = 0; vid < source_points_count; vid++)
        {
            ret.vertex_add(sps[vid], dps[vid]);
        }
        for (facet f : facets)
        {
            ret.facet_add(f);
        }

        return ret;
    }

    // double resemblance(const silhouette& them__) const
    // {

    //      auto to_silhoutte = [this]() -> vector<polygon> const {
    //         vector<polygon> ret;

    //         for (const facet& f, this->facets_) {

    //             polygon p;
    //             for (int i : f.vertex_ids_) {
    //                 p.push_back(destination_positions[i]);
    //             }
    //             ret.push_back(p);
    //         }
    //         return ret;
    //     }

    //     auto to_polygon = [](const std::vector<polygon>& polygons) {

    //         polygon_with_holes polygon;
    //         polygon_with_holes tmp;

    //         int j = 0;
    //         while (polygons[j].is_counterclockwise_oriented() == false()) {
    //             j += 1;
    //         }

    //         if (j == polygons.size()) {
    //             std::cerr << "no positive size polygons";
    //             assert(j == polygons.size());
    //         }

    //         polygon = polygons[j];

    //         for (int i = 0; i < polygons.size(); i += 1) {

    //             if (i == j) { continue; }

    //             if (p.is_counterclockwise_oriented()) {
    //                 CGAL::join(target, p, tmp);
    //             } else {
    //                 std::vector<polygon_with_holes> v;
    //                 CGAL::intersection(target, p, begin(v));

    //             }
    //             target = tmp;
    //         }

    //         return polygon;

    //     }

    //     const auto& them_ = them__.polygons;
    //     const auto& us_ = to_silhoutte();

    //     polygon_with_holes them = to_polygon(them_);
    //     polygon_with_holes us   = to_polygon(us_);

    //     polygon_with_holes both;
    //     CGAL::join(target)

    // //double resemblance(polygon target) const
    // //{
    // //    polygon_with_holes p_or;
    // //    std::vector<polygon_with_holes> p_ands;
    // //    CGAL::join(poly, target, p_or);
    // //    CGAL::intersection(poly, target, std::back_inserter(p_ands));
    // //    CGAL::Gmpq and_area = 0;
    // //    for (const auto& holy : p_ands)
    // //    {
    // //        and_area += holy.outer_boundary().area();
    // //    }
    // //    auto or_area = p_or.outer_boundary().area();
    // //    return gmpq_to_double(and_area / or_area);
    // //}
};

inline std::ostream& operator<<(std::ostream& os, const solution& s)
{
    assert(s.source_positions().size() == s.destination_positions().size());
    os << s.source_positions().size() << "\n";
    for (const auto& vertex : s.source_positions())
    {
        os << point_to_string(vertex) << "\n";
    }

    os << s.facets().size() << "\n";
    for (const auto& facet : s.facets())
    {
        os << facet.size() << " ";
        for (auto vertex_id : facet.vertex_ids())
        {
            os << vertex_id << " ";
        }
        os << "\n";
    }

    for (const auto& vertex : s.destination_positions())
    {
        os << point_to_string(vertex) << "\n";
    }
    return os;
}
}

#endif // SOLVER_SOLUTION_HPP
