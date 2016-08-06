#include <iostream>

#include <zebra/log.hpp>
#include <zebra/task.hpp>

#include <boost/program_options.hpp>
#include <zebra/solver.hpp>
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
    po::positional_options_description positional_options;
    positional_options.add("task-file", -1);

    po::options_description options("");
    // clang-format off
    options.add_options()("help", "show help message")
            ("task-file", po::value<std::string>(), "File containing the task.")
            ("runtime", po::value<long>(), "runtime")
            ("visualize", po::value<std::string>(), "visualize")
            ("verbosity,v", po::value<std::string>()->default_value("info"),
                            "set the verbosity level")
            ("cores", po::value<int>(), "core count");
    // clang-format on

    po::variables_map vm;

    try
    {
        po::store(po::command_line_parser(argc, argv)
                      .options(options)
                      .positional(positional_options)
                      .run(),
                  vm);

        po::notify(vm);
    }
    catch (std::exception& e)
    {
        // something went wrong, so lets print out the message and a usage
        std::cerr << argv[0] << ": " << e.what() << std::endl << std::endl;
        help(options);
        return 1;
    }

    zebra::log::set_min_severity_level(vm["verbosity"].as<std::string>());

    if (vm.count("help"))
    {
        help(options);
        return EXIT_SUCCESS;
    }

    if (vm.count("task-file") == 0)
    {
        help(options);
        return -1;
    }

    auto filename = vm["task-file"].as<std::string>();
    zebra::logging::info() << "filename: " << filename;
    // do something useful
    auto t = zebra::read_task(filename);
    std::unique_ptr<zebra::solver> solve = std::make_unique<zebra::simple>();

    zebra::logging::info() << "solving...";
    auto solu = (*solve)(t);
    zebra::logging::info() << "solution found...";

    if (vm.count("visualize") > 0)
    {
        solu.to_png(filename + ".png");
    }

    // std::ofstream ofs(filename_out);
    // ofs << solu;
    std::cout << solu;

    return 0;
}
