#ifndef OPENBSW_LOGGER_CONFIG_H
#define OPENBSW_LOGGER_CONFIG_H

#include "openbsw/logger/logger.h"

#include <cstdint>

struct CarAccess;
struct Authentication;

namespace openbsw
{
namespace logger
{
namespace config
{

template<>
struct ApplicationDetails<CarAccess>
{
    static inline constexpr uint8_t Id = 42U;
    //static inline constexpr const char* AbbreviationString = "TCTX";
    //static inline constexpr char Abbreviation[5] = { 'T', 'C', 'T', 'X', '\0' };
    //static inline constexpr size_t Rate = 999;
};

template<>
struct ContextDetails<Authentication>
{
    static inline constexpr uint8_t Id = 0U;
    //static inline constexpr const char* AbbreviationString = "TCTX";
    //static inline constexpr char Abbreviation[5] = { 'T', 'C', 'T', 'X', '\0' };
    //static inline constexpr size_t Rate = 999;
};

}
}
}

#endif
