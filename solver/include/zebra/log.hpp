#pragma once

#include <nitro/log/log.hpp>

#include <nitro/log/sink/stderr_mt.hpp>

#include <nitro/log/attribute/message.hpp>
#include <nitro/log/attribute/severity.hpp>
#include <nitro/log/attribute/timestamp.hpp>

#include <nitro/log/filter/and_filter.hpp>
#include <nitro/log/filter/severity_filter.hpp>

#include <zebra/exception.hpp>

#include <vector>

namespace zebra
{
namespace log
{

    namespace detail
    {

        typedef nitro::log::record<nitro::log::message_attribute, nitro::log::timestamp_attribute,
                                   nitro::log::severity_attribute>
            record;

        template <typename Record>
        class zebra_log_formater
        {
        public:
            std::string format(Record& r)
            {
                std::stringstream s;

                s << "[" << r.timestamp().count() << "][" << r.severity() << "]: " << r.message();

                return s.str();
            }
        };

        template <typename Record>
        using zebra_log_filter = nitro::log::filter::severity_filter<Record>;
    }

    typedef nitro::log::logger<detail::record, detail::zebra_log_formater,
                               nitro::log::sink::stderr_mt, detail::zebra_log_filter>
        logging;

    inline void set_min_severity_level(nitro::log::severity_level sev)
    {
        detail::zebra_log_filter<detail::record>::set_severity(sev);
    }

    inline void set_min_severity_level(std::string verbosity)
    {
        if (verbosity == "trace")
        {
            set_min_severity_level(nitro::log::severity_level::trace);
            return;
        }

        if (verbosity == "debug")
        {
            set_min_severity_level(nitro::log::severity_level::debug);
            return;
        }

        if (verbosity == "info")
        {
            set_min_severity_level(nitro::log::severity_level::info);
            return;
        }

        if (verbosity == "warn")
        {
            set_min_severity_level(nitro::log::severity_level::warn);
            return;
        }

        if (verbosity == "error")
        {
            set_min_severity_level(nitro::log::severity_level::error);
            return;
        }

        if (verbosity == "fatal")
        {
            set_min_severity_level(nitro::log::severity_level::fatal);
            return;
        }

        zebra::raise("Unknown verbosity level given");
    }

} // namespace log

using log::logging;

} // namespace zebra

namespace std
{
template <typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& v)
{
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        o << v[i];
        if (i < v.size() - 1)
        {
            o << ",";
        }
    }
    return o;
}
}
