#include <iostream>

#include <zebra/log.hpp>
#include <zebra/task.hpp>

#include <boost/program_options.hpp>
#include <zebra/solver.hpp>
#include <zebra/solvers/solve_backward.hpp>
#include <zebra/solvers/simple.hpp>
#include <zebra/solvers/stupid.hpp>

namespace po = boost::program_options;

void help(po::options_description& desc)
{
    std::cerr << "Synopsis\n"
                 "=========\n"
              << desc << std::endl;
}

int main(int argc, char** argv)
{
    zebra::logging::info() << "Hello, World!";

    po::positional_options_description positional_options;
    positional_options.add("task-file", -1);

    po::options_description options("");
    // clang-format off
    options.add_options()("help", "show help message")
            ("task-file", po::value<std::string>(), "File containing the task.")
            ("runtime", po::value<long>(), "runtime")
            ("cores", po::value<int>(), "core count");
    // clang-format on

    po::variables_map vm;

    po::store(
        po::command_line_parser(argc, argv).options(options).positional(positional_options).run(),
        vm);

    if (vm.count("help"))
    {
        help(options);
        return EXIT_SUCCESS;
    }

    po::notify(vm);

    auto filename = vm["task-file"].as<std::string>();
    zebra::logging::info() << "filename: " << filename;
    // do something useful
    auto t = zebra::read_task(filename);
    zebra::node_graph ng(t);
    zebra::print_node_graph(ng);
    zebra::backward b;
    b( t );
    //std::unique_ptr<zebra::solver> solve = std::make_unique<zebra::simple>();

    //zebra::logging::info() << "solving...";
    //auto solu = (*solve)(t);
    //zebra::logging::info() << "solution found...";

    // std::ofstream ofs(filename_out);
    // ofs << solu;
    //std::cout << solu;


    return 0;
}
