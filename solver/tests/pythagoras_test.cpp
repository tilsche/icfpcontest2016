#include <zebra/pythagoras.hpp>

#include <iostream>

int main()
{
    for (auto t : zebra::get_pythagorean_triples())
    {
        std::cout << t[0] << ", " << t[1] << ", " << t[2] << std::endl;
    }
}
