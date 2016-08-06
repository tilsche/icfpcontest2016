#include <zebra/log.hpp>

#include <vector>

int main()
{
    std::vector<int> v;

    zebra::logging::info() << "Hello, World!";

    zebra::logging::info() << v;

    return 0;
}
