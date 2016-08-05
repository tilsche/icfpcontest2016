#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

namespace zebra
{
struct exception : std::runtime_error
{
    explicit exception(const std::string& arg) : std::runtime_error(arg)
    {
    }
};

namespace detail
{

    template <typename Arg, typename... Args>
    class make_exception
    {
    public:
        void operator()(std::stringstream& msg, Arg arg, Args... args)
        {
            msg << arg;
            make_exception<Args...>()(msg, args...);
        }
    };

    template <typename Arg>
    class make_exception<Arg>
    {
    public:
        void operator()(std::stringstream& msg, Arg arg)
        {
            msg << arg;
        }
    };
}

template <typename... Args>
inline void raise(Args... args)
{
    std::stringstream msg;

    detail::make_exception<Args...>()(msg, args...);

    throw exception(msg.str());
}
} // namespace zebra
