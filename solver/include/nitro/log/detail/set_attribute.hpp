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

#ifndef INCLUDE_NITRO_LOG_DETAIL_SET_ATTRIBUTE_HPP
#define INCLUDE_NITRO_LOG_DETAIL_SET_ATTRIBUTE_HPP

#include <nitro/log/attribute/severity.hpp>
#include <nitro/log/detail/has_attribute.hpp>
#include <nitro/log/record.hpp>
#include <nitro/log/severity.hpp>

namespace nitro
{
namespace log
{
    namespace detail
    {

        template <bool, template <typename... Attributes> class Record, typename... Attributes>
        struct assign_severity
        {
            void operator()(Record<Attributes...>& r, const severity_level& v)
            {
                r.severity() = v;
            }
        };

        template <typename... Attributes, template <typename... Attributes2> class Record>
        struct assign_severity<false, Record, Attributes...>
        {
            void operator()(Record<Attributes...>&, const severity_level&)
            {
            }
        };

        template <typename... Attributes>
        struct set_severity;

        template <typename... Attributes>
        struct set_severity<record<Attributes...>>
        {
            void operator()(record<Attributes...>& r, const severity_level& v)
            {
                assign_severity<has_attribute<severity_attribute, record<Attributes...>>::value,
                                record, Attributes...>()(r, v);
            }
        };
    }
}
} // namespace nitro::log::detail

#endif // INCLUDE_NITRO_LOG_DETAIL_SET_ATTRIBUTE_HPP
