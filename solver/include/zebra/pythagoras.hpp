#ifndef SOLVER_PYTHAGORAS_HPP
#define SOLVER_PYTHAGORAS_HPP

#include <array>
#include <vector>

namespace zebra
{
std::vector<std::array<int, 3>> compute_pythagorean_triples(int max)
{
    std::vector<std::array<int, 3>> result;
    for (int a = 1; a <= max; a++)
    {
        for (int b = a; b <= max; b++)
        {
            for (int c = b; c <= max; c++)
            {
                if ((a * a) + (b * b) == c * c && c <= max)
                {
                    result.emplace_back(std::array<int, 3>{ { a, b, c } });
                }
            }
        }
    }
    return result;
};

const std::vector<std::array<int, 3>>& get_pythagorean_triples()
{
    static auto t = compute_pythagorean_triples(1000);
    return t;
};
}

#endif // SOLVER_PYTHAGORAS_HPP
