#ifndef OPENBSW_LOGGER_LOGGER_H
#define OPENBSW_LOGGER_LOGGER_H

#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>

namespace openbsw
{
namespace logger
{

namespace config
{

template<typename T>
struct ApplicationDetails
{
};

template<typename T>
struct ContextDetails
{
};

}

constexpr uint32_t Abbreviation(const char (&str)[5])
{
    return (str[0] << 24) | (str[1] << 16) | (str[2] << 8) | (str[3] << 0);
}

constexpr uint16_t hash16(const char *string, uint32_t length)
{
    constexpr uint16_t BASE_HASH = 5381U;
    constexpr uint16_t HASH_MULTIPLICATION_VALUE = 33U;
    uint16_t result = BASE_HASH;

    for (uint32_t i = 0U; i < length; i++)
    {
        result = ((result * HASH_MULTIPLICATION_VALUE) + string[i]) % 0xFFFFU;
    }

    return result;
}

template <uint32_t C>
constexpr uint16_t HashedId(const char (&string)[C])
{
    return hash16(&string[0], C);
}

enum class LogLevel : uint8_t
{
    DEBUG,
    INFO,
    ERROR,
};

template<uint32_t ABBR>
struct Application
{
    static inline constexpr char Abbreviation[5] = { (ABBR >> 24) & 0xff, (ABBR >> 16) & 0xff, (ABBR >> 8) & 0xff, (ABBR >> 0) & 0xff, '\0' };
};

template<typename APP, uint32_t ABBR>
struct Context
{
    using Application = APP;
    static inline constexpr char Abbreviation[5] = { (ABBR >> 24) & 0xff, (ABBR >> 16) & 0xff, (ABBR >> 8) & 0xff, (ABBR >> 0) & 0xff, '\0' };
};

template<typename T, LogLevel LEVEL, uint16_t ID>
struct Message
{
    using Context = T;
    using Application = typename T::Application;
    static inline constexpr uint16_t Id = ID;
    static inline constexpr LogLevel Level = LEVEL;
    void format(char *buffer, size_t size)
    {
        std::snprintf(buffer, size, "[%d] binary data ...", Id);
    }
};

#define MESSAGE(name, component, level) \
struct __attribute__((packed)) name : public ::rcore::logger::Message<component, ::rcore::logger::LogLevel::level, ::rcore::logger::HashedId(#name)>

template<typename M>
void log(M m)
{
    // dummy implementation to log on the console

    uint8_t buffer[sizeof(M)];
    std::memcpy(buffer, &m, sizeof(m));

    constexpr auto ContextId = config::ContextDetails<typename M::Context>::Id;
    constexpr auto ApplicationId = config::ApplicationDetails<typename M::Application>::Id;

    std::cout
        << std::dec << "["
        << typeid(typename M::Application).name() << "(" << (int)config::ApplicationDetails<typename M::Application>::Id << "):"
        << typeid(typename M::Context).name() << "(" << (int)config::ContextDetails<typename M::Context>::Id << "):"
        << typeid(M).name() << "(" << (int)M::Id << ")"
        << "]" << std::endl;

    std::cout << "  = binary >";
    for (size_t i = 0U; i < sizeof(m); i++)
    {
        std::cout << " 0x" << std::hex << (int)buffer[i];
    }
    std::cout << std::endl;

    char format[100];
    m.format(format, sizeof(format));
    format[sizeof(format) - 1] = '\0';
    std::cout << "  = format > " << std::string(format);
    std::cout << std::endl;
}

template<typename M, typename... A>
void logArgs(A&&... args)
{
    M m { {}, std::forward<A>(args)... };
    log(m);
}

template <typename M>
void logFill(std::function<void(M &)> fill)
{
    M m {};
    fill(m);
    log(m);
}

}
}

#endif
