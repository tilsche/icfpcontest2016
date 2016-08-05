/*
 * Copyright (c) 2015-2016, Technische Universität Dresden, Germany
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INCLUDE_NITRO_ARGS_HPP
#define INCLUDE_NITRO_ARGS_HPP

#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace nitro
{
namespace args
{

    class parser
    {
    public:
        parser(int argc, char** argv) : argc(argc), argv(argv)
        {
            bool only_unnamed = false;

            for (int i = 1; i < argc; i++)
            {
                std::string arg = argv[i];

                if (is_double_dash(arg))
                {
                    only_unnamed = true;
                    continue;
                }

                if (is_unnamed(arg) || only_unnamed)
                {
                    unnamed.push_back(arg);
                    continue;
                }

                if (is_short_named(arg))
                {
                    for (int j = 1; j < arg.size(); j++)
                    {
                        named[std::string(&arg[j], &arg[j + 1])] = "";
                    }
                }
            }
        }

        static bool is_unnamed(const std::string& option)
        {
            return !(is_short_named(option) || is_long_named(option));
        }

        static bool is_short_named(const std::string& arg)
        {
            return arg[0] == '-' && arg[1] != '-';
        }

        static bool is_long_named(const std::string& arg)
        {
            return arg[0] == '-' && arg[1] == '-';
        }

        static bool is_double_dash(const std::string& arg)
        {
            return arg.size() == 2 && is_long_named(arg);
        }

        const std::string& operator[](std::size_t i) const
        {
            return unnamed[i];
        }

        const std::string& operator[](const std::string& name) const
        {
            return named[name];
        }

    private:
        int argc;
        char** argv;

        std::vector<std::string> unnamed;
        std::map<std::string, std::string> named;
    };
}
} // namespace nitr::args

#endif // INCLUDE_NITRO_ARGS_HPP
