#ifndef SOLVER_FACET_HPP
#define SOLVER_FACET_HPP

#include <zebra/geometry.hpp>

#include <vector>

namespace zebra
{
using vertex_id = size_t;
using facet_id = size_t;

class facet
{
public:
    facet()
    {
    }

    // TODO universal reference
    facet(const std::vector<vertex_id>& v) : vertex_ids_(v)
    {
    }

    facet(const std::vector<vertex_id>& v, transformation t) : vertex_ids_(v), transform_(t)
    {
    }

    facet(transformation t) : transform_(t)
    {
    }

    static facet unit_facet()
    {
        return facet(std::vector<vertex_id>{ 0, 1, 2, 3 });
    }

    size_t size() const
    {
        return vertex_ids_.size();
    }

    size_t operator[](size_t index) const
    {
        return vertex_ids_[index % size()];
    }

    const std::vector<vertex_id>& vertex_ids() const
    {
        return vertex_ids();
    }

    const transformation& transform() const
    {
        return transform_;
    }

    transformation inverse() const
    {
        transformation i = transform_.inverse();
        logging::trace() << "INVERTING: " << transform_;
        logging::trace() << " = : " << i;
        logging::trace() << "ID: " << transform_ * i;
        return i;
    }

    void transform(transformation t)
    {
        logging::trace() << "Adding transformation: " << t << " to " << transform_;
        // transform_ = transform_ * t;
        transform_ = t * transform_;
        logging::trace() << "= " << transform_;
    }

    std::vector<vertex_id> vertex_ids_;

private:
    transformation transform_;
};
}
#endif // SOLVER_FACET_HPP
