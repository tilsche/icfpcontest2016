#include <iostream>

#include <zebra/log.hpp>

#include <boost/program_options.hpp>

namespace po = boost::program_options;


void help(po::options_description& desc)
{
    std::cerr <<
        "Synopsis\n"
        "=========\n"
        << desc
        << std::endl;
}


int main(int argc, char** argv)
{
    zebra::logging::info() << "Hello, World!";

    po::options_description options("");
    options.add_options()
        ("help", "show help message")
        ("task-file", po::value<std::string>(), "File containing the task.");

    po::variables_map vm;

    po::store(po::command_line_parser(argc, argv).options(options).run(), vm);

    if (vm.count("help")) {
        help(options);
        return EXIT_SUCCESS;
    }

    po::notify(vm);

    if (vm.count("task-file")) {
        auto filename = vm["task-file"].as<std::string>();
        zebra::logging::info() << "filename: " << filename << "\n";
        // do something useful
    }

    return 0;
}
