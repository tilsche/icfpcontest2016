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

#ifndef INCLUDE_NITRO_DL_DL_HPP
#define INCLUDE_NITRO_DL_DL_HPP

#include <nitro/dl/exception.hpp>
#include <nitro/dl/symbol.hpp>

#include <dlfcn.h>

#include <memory>
#include <sstream>
#include <string>

namespace nitro
{
namespace dl
{

    /**
     * \brief Class for dynamically loading libraries
     *
     * This class uses libdl to load libraries dynamically.
     *
     */
    class dl
    {
    public:
        /**
         * \brief construct with the name of the library
         *
         *
         *
         * \param [in] filename the filename of the library {
         * libdl tries to find a library with the given name in LD_LIBRARY_PATH.
         * }
         *
         * \throws nitro::dl::exception {
         * throws if the library could not be opened.
         * }
         */
        dl(const std::string& filename)
        : handle(dlopen(filename.c_str(), RTLD_NOW), [](void* handle) { dlclose(handle); })
        {
            if (handle == nullptr)
            {
                std::stringstream msg;
                msg << "Couldn't open library '" << filename << '\'';

                throw nitro::dl::exception(msg.str(), dlerror());
            }
        }

        template <typename T>
        nitro::dl::symbol<T> load(const std::string& name)
        {
            return nitro::dl::symbol<T>(handle, name);
        }

        std::shared_ptr<void> get() const
        {
            return handle;
        }

    private:
        std::shared_ptr<void> handle;
    };
}
} // namespace nitro::dl

#endif // INCLUDE_NITRO_DL_DL_HPP
