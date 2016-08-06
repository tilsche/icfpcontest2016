#include <zebra/geometry.hpp>
#include <zebra/log.hpp>

using namespace zebra;
int main()
{
    auto p_left = point_from_string("-1,0");
    auto p_on = point_from_string("0,0");
    auto p_right = point_from_string("1,0");

    line l = line(point(0, -1), point(0, 1));
    logging::info() << l.has_on_positive_side(p_left);
    logging::info() << l.has_on_positive_side(p_on);
    logging::info() << l.has_on_positive_side(p_right);
    logging::info() << l.has_on_negative_side(p_left);
    logging::info() << l.has_on_negative_side(p_on);
    logging::info() << l.has_on_negative_side(p_right);
    logging::info() << l.has_on(p_left);
    logging::info() << l.has_on(p_on);
    logging::info() << l.has_on(p_right);
}
