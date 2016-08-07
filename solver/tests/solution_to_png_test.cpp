#include <zebra/solution.hpp>
using namespace std;
using namespace zebra;

int main(int argc, char** args) {

    solution s = solution::from_file(args[1]);
    s.to_png(basename(args[1]));

    return 0;
}
