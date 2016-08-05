#include <iostream>

#include <zebra/log.hpp>
#include <zebra/task.hpp>

#include <boost/program_options.hpp>
#include <zebra/solver.hpp>
#include <zebra/stupidsolver.hpp>

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

    po::options_description options("");
    // clang-format off
    options.add_options()("help", "show help message")
            ("task-file", po::value<std::string>(), "File containing the task.")
            ("output-file", po::value<std::string>(), "File containing the output.");
    // clang-format on

    po::variables_map vm;

    po::store(po::command_line_parser(argc, argv).options(options).run(), vm);

    if (vm.count("help"))
    {
        help(options);
        return EXIT_SUCCESS;
    }

    po::notify(vm);

    assert(vm.count("task-file"));
    assert(vm.count("output-file"));

    auto filename = vm["task-file"].as<std::string>();
    auto filename_out = vm["output-file"].as<std::string>();
    zebra::logging::info() << "filename: " << filename << "\n";
    // do something useful
    auto t = zebra::read_task(filename);
    std::unique_ptr<zebra::solver> solve = std::make_unique<zebra::stupidsolver>();
    auto solu = (*solve)(t);

    std::ofstream ofs(filename_out);
    ofs << solu;

    return 0;
}
