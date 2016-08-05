#ifndef __ZEBRA_TASK_HPP__
#define __ZEBRA_TASK_HPP__

#include <iostream>
#include <ssteam>
#include <functional>
#include <vector>

#include <boost/algorithm/sting.hpp>
#include <boost/filesystem.hpp>

#include "point.hpp"


namespace zebra {

bool for_each_line(const std::string& filename, std::function<bool(const std::string&)> cb)
{
    std::ifstream in{filename};

    for (std::string line; getline(in, line);) {
        if (! cb(line)) {
            break;
        }
    }
    return true;
}


std::vector<point> read_task(const std::string& taskfile)
{
    std::vector<point> data;

    for_each_line(taskfile, [&](const std::string& line) {
            data.emplace_back(point::from_string(line));
        });

    return data;
}


} /* closing namespace zebra */

#endif /* __ZEBRA_TASK_HPP__ */
