#ifndef ZEBRA_POINT_HPP
#define ZEBRA_POINT_HPP

#include "cgal.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

namespace zebra
{

inline point point_from_string(std::string s)
{
    for (std::size_t i = 0; i < s.size(); i += 1)
    {
        if (s[i] == ',')
        {
            s[i] = ' ';
        }
    }
    std::stringstream ss(s);
    CGAL::Gmpq x, y;
    ss >> x;
    ss >> y;
    return point(x, y);
}

std::string gmpq_to_string(const CGAL::Gmpq& q) {
    std::stringstream ss;
    ss << q;
    std::string s = ss.str();

    if (s.size() < 3) { return s; }

    if (s[s.size()-2] == '/' && s[s.size()-1] == '1') {
        return s.substr(0, s.size()-2);
    } else {
        return s;
    }
}

std::string point_to_string(const point& p) {
    std::stringstream s;
    s << gmpq_to_string(p.x()) << "," << gmpq_to_string(p.y());
    return s.str();
}

}

#endif // ZEBRA_POINT_HPP
