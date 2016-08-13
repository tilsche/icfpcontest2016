#include <zebra/log.hpp>
#include <zebra/task.hpp>

#include <boost/program_options.hpp>
#include <zebra/solvers/base.hpp>
#include <zebra/solvers/meta.hpp>

#include <chrono>
#include <iostream>

namespace po = boost::program_options;

void help(po::options_description& desc)
{
    std::cerr << "Synopsis\n"
                 "=========\n"
              << desc << std::endl;
}

int main(int argc, char** argv)
{
    std::string file_name;
    std::string verbosity_name = "info";
    int core_count;
    long runtime_ms = 10000;
    bool visualize = false;
    std::string solver_name = "bfs";

    po::positional_options_description positional_options;
    positional_options.add("task-file", -1);

    po::options_description options("");
    // clang-format off
    options.add_options()("help", "show help message")
            ("task-file,f", po::value(&file_name)->required(), "path to file containing the task")
            ("runtime,t", po::value(&runtime_ms), "runtime in milliseconds")
            ("visualize,z", po::bool_switch(&visualize), "visualize the solutoin")
            ("verbosity,v", po::value(&verbosity_name),
                            "verbosity level (error,warn,info,debug,trace)")
            ("cores,c", po::value(&core_count), "core count (currently not supported)")
            ("solver,s", po::value(&solver_name),
                         "select solver (stupid, simple, idfs, bfs)");
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

    zebra::log::set_min_severity_level(verbosity_name);

    if (vm.count("help"))
    {
        help(options);
        return EXIT_SUCCESS;
    }
    auto runtime = std::chrono::milliseconds(long(runtime_ms * 0.95));

    if (vm.count("task-file") == 0)
    {
        help(options);
        return -1;
    }

    // do something useful
    auto t = zebra::read_task(file_name);

    zebra::solver::meta solve(solver_name, runtime);

    zebra::logging::info() << "solving...";
    auto solu = solve(t);
    zebra::logging::info() << "solution found...";

    if (vm.count("visualize") > 0)
    {
        solu.to_png(file_name + "_" + solver_name + ".png");
    }

    // std::ofstream ofs(filename_out);
    // ofs << solu;
    std::cout << solu.resemblance(t);
    std::cout << solu;

    return 0;
}
