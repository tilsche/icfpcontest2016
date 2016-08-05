#ifndef ZEBRA_LINE_SEGMENT
#define ZEBRA_LINE_SEGMENT

#include "kernel.hpp"
#include <zebra/geometry.hpp>

namespace zebra
{

using line_segment = kernel::Segment_2;

}
/*
{
public:
    point a, b;

    line_segment() {}
    line_segment(const line_segment& s) : a(s.a), b(s.b) {}
    line_segment(const point& a_, const point& b_) : a(a_), b(b_) {}

    line_segment operator+(const point& rhs) const
    {
        return line_segment(a + rhs, b + rhs);
    }
    line_segment operator-(const point& rhs) const
    {
        return line_segment(a - rhs, b - rhs);
    }
    line_segment operator*(const mpq_class f) const
    {
        return line_segment(a * f, b * f);
    }
    bool operator==(const line_segment& rhs) const
    {
        return a == rhs.a && b == rhs.b;
    }
    bool operator!=(const line_segment& rhs) const
    {
        return !(*this == rhs);
    }

    line_segment& operator=(const line_segment& rhs) {
        a = rhs.a;
        b = rhs.b;
        return *this;
    }

    bool slope_inf() const {
        return (a - b).x == 0;
    }

    mpq_class slope() const {
        assert(slope_inf() == false);

        point aa, bb;
        if (a.x <= b.x) {
            aa = a;
            bb = b;
        } else {
            aa = b;
            bb = a;
        }

        return (bb.y - aa.y) / (bb.x - aa.x);
    }

    static line_segment from_string(const std::string& s)
    {

        int space = -1;
        for (int i = 0; i < s.size(); i += 1)
        {
            if (space == -1 && s[i] == ' ')
            {
                space = i;
            }

            assert(s[i] == ',' || s[i] == '/' || s[i] == ' ' || s[i] == '-' || (s[i] >= '0' && s[i] <= '9'));
            assert(! (space != -1 && s[i] == ' ' && space != i));
        }

        assert(space != -1);

        std::string a = s.substr(0, space);
        std::string b = s.substr(space + 1, s.size() - (space + 1));

        return line_segment(point::from_string(a), point::from_string(b));
    }
};

bool is_disjoint(const line_segment& l, const line_segment& m) {
    if (l.slope_inf() && m.slope_inf()) {
        // pass
    } else if (l.slope_inf() || m.slope_inf()) {
        return false;
    } else if (l.slope() != m.slope()) {
        return false
    }



}

return false;
}

std::ostream& operator<<(std::ostream& o, const line_segment& s) {
    o << s.a << " " << s.b;
    return o;
}

};
*/
#endif // ZEBRA_LINE_SEGMENT
