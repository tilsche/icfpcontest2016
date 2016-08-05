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

#ifndef INCLUDE_NITRO_LOG_FILTER_SEVERITY_FILTER_HPP
#define INCLUDE_NITRO_LOG_FILTER_SEVERITY_FILTER_HPP

#include <nitro/log/severity.hpp>

namespace nitro
{
namespace log
{
    namespace filter
    {

        template <typename Record, unsigned N = 0>
        class severity_filter
        {
        public:
            typedef Record record_type;

            static void set_severity(severity_level new_sev)
            {
                sev = new_sev;
            }

            static severity_level min_severity()
            {
                return sev;
            }

            bool filter(Record& r) const
            {
                return r.severity() >= min_severity();
            }

        private:
            static severity_level sev;
        };

        template <typename Record, unsigned N>
        severity_level severity_filter<Record, N>::sev = severity_level::trace;
    }
}
} // namespace nitro::log::filter

#endif // INCLUDE_NITRO_LOG_FILTER_SEVERITY_FILTER_HPP
