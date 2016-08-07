#ifndef ZEBRA_UPOINT_HPP
#define ZEBRA_UPOINT_HPP

#include "point.hpp"

namespace zebra
{
    struct upoint : point
    {
        upoint();
        upoint(point p);
        upoint(CGAL::Gmpq, CGAL::Gmpq);
        unsigned int id() const;
        bool operator==(const upoint&) const;
        bool operator!=(const upoint&) const;

        private:
        point m_src;
        unsigned int m_id;
        static unsigned int id_init;
    };
}

#endif /* ZEBRA_UPOINT_HPP */
