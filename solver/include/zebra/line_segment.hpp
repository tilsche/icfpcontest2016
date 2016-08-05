#ifndef ZEBRA_LINE_SEGMENT
#define ZEBRA_LINE_SEGMENT

#include <zebra/point.hpp>

namespace zebra {

class line_segment {
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

    static line_segment from_string(const std::string& s)
    {

        int space = -1;
        for (int i = 0; i < s.size(); i += 1)
        {
            if (space == -1 && s[i] == ' ')
            {
                space = i;
            }

            assert(s[i] == ',' || s[i] == '/' || s[i] == ' ' || (s[i] >= '0' && s[i] <= '9'));
            assert(! (space != -1 && s[i] == ' ' && space != i));
        }

        assert(space != -1);

        std::string a = s.substr(0, space);
        std::string b = s.substr(space + 1, s.size() - (space + 1));

        return line_segment(point::from_string(a), point::from_string(b));
    }
};

std::ostream& operator<<(std::ostream& o, const line_segment& s) {
    o << s.a << " " << s.b;
    return o;
}

};

#endif // ZEBRA_LINE_SEGMENT
