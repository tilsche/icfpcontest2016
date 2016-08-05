#ifndef ZEBRA_POINT_HPP
#define ZEBRA_POINT_HPP

#include <cassert>
#include <gmpxx.h>
#include <iostream>
#include <string>

namespace zebra
{

class point
{
public:
    mpq_class x, y;

    point() : x(-1), y(-1)
    {
    }
    point(const point& p) : x(p.x), y(p.y)
    {
    }
    point(const mpq_class& x_, const mpq_class& y_) : x(x_), y(y_)
    {
    }

    point operator=(const point& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    point operator+(const point& rhs) const
    {
        return point(x + rhs.x, y + rhs.y);
    }
    point operator-(const point& rhs) const
    {
        return point(x - rhs.x, y - rhs.y);
    }
    point operator*(const mpq_class f) const
    {
        return point(x * f, y * f);
    }
    bool operator==(const point& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const point& rhs) const
    {
        return !(*this == rhs);
    }

    static point from_string(const std::string& s)
    {

        int comma = -1;
        for (int i = 0; i < s.size(); i += 1)
        {
            if (comma == -1 && s[i] == ',')
            {
                comma = i;
            }

            assert(s[i] == ',' || s[i] == '/' || (s[i] >= '0' && s[i] <= '9'));
        }

        assert(comma != -1);

        std::string x = s.substr(0, comma);
        std::string y = s.substr(comma + 1, s.size() - (comma + 1));

        point p;
        mpq_set_str(p.x.get_mpq_t(), x.c_str(), 10);
        mpq_set_str(p.y.get_mpq_t(), y.c_str(), 10);

        return p;
    }
};

using vector = point;

std::ostream& operator<<(std::ostream& o, const point& p)
{
    o << p.x << "," << p.y;
    return o;
}
};

#endif // ZEBRA_POINT_HPP
