#ifndef STRINGID_H
#define STRINGID_H

#include <cstdint>
#include <rapidhash/rapidhash.h>
#include <string_view>

namespace Engine
{
    using StringIdType = std::uint64_t;

    struct StringId {
        std::string_view str{};
        StringIdType id{};
    };

#define SID(str) str##_sid

    constexpr StringIdType hash(std::string_view str, std::size_t len) { return rapidhash(str.data(), len); }

    constexpr StringId operator""_sid(const char* str, std::size_t len)
    {
        return StringId{str, hash(str, len)};
    }

    StringId internString(std::string_view str);
} // namespace Engine

#endif
