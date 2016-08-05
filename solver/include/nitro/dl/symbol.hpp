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

#ifndef INCLUDE_NITRO_DL_SYMBOL_HPP
#define INCLUDE_NITRO_DL_SYMBOL_HPP

#include <nitro/dl/exception.hpp>

#include <dlfcn.h>

#include <memory>
#include <sstream>
#include <string>

namespace nitro
{
namespace dl
{

    /**
     * \brief Class for holding and calling a handler to a dynamically loaded
     *symbol in a typesafe way.
     *
     *
     * \internal
     *
     * This nice trick to get return and argument types out of the instantiation
     * with 'symbol<Ret(Args...)>' is copied from std::function :-)
     *
     */
    template <typename T>
    class symbol;

    /**
     * \brief Class for holding and calling a handler to a dynamically loaded
     *symbol in a typesafe way.
     *
     * \attention Don't let the parental object of type \ref nitro::dl::dl go
     *out of
     * scope. This will fuck up everything.
     */
    template <typename Ret, typename... Args>
    class symbol<Ret(Args...)>
    {
    public:
        /**
         * \brief Constructor for a symbol
         *
         * This constructor gets called by the parental dl instance. You should
         * not call it yourself.
         *
         * \internal
         *
         * \param library the pointer to the \ref nitro::dl::dl instance
         * \param name the name of the symbol which is to be called
         *
         */
        symbol(std::shared_ptr<void> library, const std::string& name) : library(library)
        {
            // clear previous errors
            dlerror();

            // dirty cast copied from http://linux.die.net/man/3/dlopen
            *(void**)(&this->handle) = dlsym(library.get(), name.c_str());

            // read out dlerror and check it against NULL, as it's legal for
            // dlsym to return NULL
            char* error = dlerror();
            if (error != nullptr)
            {
                std::stringstream msg;
                msg << "Couldn't open symbol '" << name << '\'';

                throw nitro::dl::exception(msg.str(), error);
            }
        }

        /**
         * \brief Typesafe call of dynamic loaded function
         *
         *
         */
        Ret operator()(Args... args)
        {
            return (*handle)(args...);
        }

    private:
        Ret (*handle)(Args...);

        std::shared_ptr<void> library;
    };
}
} // namespace nitr::args

#endif // INCLUDE_NITRO_DL_SYMBOL_HPP
