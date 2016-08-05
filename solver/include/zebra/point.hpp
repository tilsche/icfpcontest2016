#ifndef ZEBRA_POINT_HPP
#define ZEBRA_POINT_HPP

#include <cassert>
#include <gmpxx.h>
#include <iostream>
#include <string>

namespace zebra {

class point {
    public:
        mpq_class x, y;


        static point from_string(const std::string& s) {

            int first_slash = -1, comma = -1, second_slash = -1;
            for (int i = 0; i < s.size(); i += 1) {
                if (first_slash != -1 && comma == -1 && s[i] == ',') {
                    comma = i;
                }

                assert(s[i] == ',' || s[i] == '/' || (s[i] >= '0' && s[i] <= '9'));
            }

            assert(comma        != -1);

            std::string x = s.substr(0, comma);
            std::string y = s.substr(comma+1, s.size() - (comma+1));

            point p;
            mpq_set_str(p.x.get_mpq_t(), x.c_str(), 10);
            mpq_set_str(p.y.get_mpq_t(), y.c_str(), 10);

            return p;
        }


};

std::ostream& operator<<(std::ostream& o, const point& p) {
    o << p.x << "," << p.y;

    return o;
}

};

#endif // ZEBRA_POINT_HPP
