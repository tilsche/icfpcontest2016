#include <zebra/task.hpp>

namespace zebra
{
void check_silhouette(const std::string& taskname)
{
    auto t = read_task(taskname);
    // std::cout << "Polygons: " << t.sil.polygons().size() << std::endl;
    int cw = 0;
    int ccw = 0;
    for (const auto& p : t.sil.polygons())
    {
        if (p.is_counterclockwise_oriented())
        {
            ccw++;
        }
        else
        {
            assert(p.is_clockwise_oriented());
            cw++;
        }
    }
    int holes = t.sil.shape().number_of_holes();

    if ( // t.sil.polygons().size() != 1 || ccw != 1 || cw != 0 ||
        holes != 0)
    {
        std::cout << taskname << std::endl;
        std::cout << "CCW: " << ccw << std::endl;
        std::cout << "CW: " << cw << std::endl;
        std::cout << "Holes: " << holes << std::endl;
    }
}
}

int main(int argc, const char* argv[])
{
    assert(argc == 2);
    zebra::check_silhouette(argv[1]);
}
