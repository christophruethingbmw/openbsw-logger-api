#include "openbsw/logger/logger.h"
#include "config.h"

#include <cstdint>

enum class AuthType : uint8_t
{
    BEGIN,
    END,
    OTHER,
};

struct CarAccess : public ::openbsw::logger::Application<::openbsw::logger::Abbreviation("CAAC")> {};
struct Authentication : public ::openbsw::logger::Context<CarAccess, ::openbsw::logger::Abbreviation("AUTH")> {};

struct __attribute__((packed)) AuthEnd : public ::openbsw::logger::Message<
    Authentication, ::openbsw::logger::LogLevel::DEBUG, ::openbsw::logger::HashedId("AuthEnd")>
{
    uint8_t transactionId;
    AuthType authType;
    uint32_t authData;

    void format(char *buffer, size_t size)
    {
        std::snprintf(buffer, size, "authentication for transaction %u with data %u", transactionId, authData);
        //return std::format("authentication for transaction {0} with data {1}", +transactionId, +authData);
    }
};

int main(int argc, char** argv)
{
    ::openbsw::logger::logArgs<AuthEnd>((uint8_t)1U, AuthType::END, 0xAFFEDEADU);
}
