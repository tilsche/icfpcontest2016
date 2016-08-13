#ifndef ZEBRA_LINE_SEGMENT
#define ZEBRA_LINE_SEGMENT

#include <sstream>

#include "cgal.hpp"
#include "point.hpp"

namespace zebra
{

inline line_segment line_segment_from_string(const std::string& s)
{
    int space = -1;
    for (std::size_t i = 0; i < s.size(); i += 1)
    {
        if (space == -1 && s[i] == ' ')
        {
            space = (int) i;
        }

        assert(s[i] == ',' || s[i] == '/' || s[i] == ' ' || s[i] == '-' ||
               (s[i] >= '0' && s[i] <= '9'));
        assert(!(space != -1 && s[i] == ' ' && space != (int) i));
    }

    assert(space != -1);

    std::string a = s.substr(0, space);
    std::string b = s.substr(space + 1, s.size() - (space + 1));

    return line_segment(point_from_string(a), point_from_string(b));
}

std::string line_segment_to_string(const line_segment& l) {
    std::stringstream s;
    s << point_to_string(l.source()) << " " << point_to_string(l.target());
    return s.str();
}

bool is_disjoint(const line_segment& l, const line_segment& m) {

    if (l == m) {
        return false;
    }

    //std::cerr << l.direction() << ", " << m.direction() << std::endl;

    if (l.direction() == m.direction() || l.opposite().direction() == m.direction()) {
        //std::cerr << l.has_on(m.source()) << l.has_on(m.target());
        if (l.has_on(m.source())) {
            if (l.source() != m.source() && l.target() != m.source()) {
                return false;
            }
        } else if (l.has_on(m.target())) {
            if (l.source() != m.target() && l.target() != m.target()) {
                return false;
            }
        }
    } else {
        return true;
    }

    return true;
}

}

#endif // ZEBRA_LINE_SEGMENT
