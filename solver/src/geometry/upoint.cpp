#include <zebra/geometry/upoint.hpp>

namespace zebra {

    upoint::upoint() : m_id(id_init++)
    {
    }

    upoint::upoint(point p) : point(p), m_src(p), m_id(id_init++)
    {
    }

    upoint::upoint(CGAL::Gmpq a, CGAL::Gmpq b) : point(a,b), m_src(a,b), m_id(id_init++)
    {
    }

    unsigned int upoint::id() const
    {
        return m_id;
    }

    bool upoint::operator==(const upoint& other) const
    {
        return m_id == other.id();
    }

    bool upoint::operator!=(const upoint& other) const
    {
        return !(*this == other);
    }

    unsigned int upoint::id_init = 0;

}
