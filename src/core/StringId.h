#ifndef STRINGID_H
#define STRINGID_H

#include "Hash.h"
#include <cstdint>
#include <string>
#include <string_view>

namespace Engine
{
    using StringIdType = std::uint32_t;

    /**
     * Class that wraps a string and its hashed value. The idea is to be able to use strings as IDs without
     * the performance penalty of string comparison.
     */
    class StringId
    {
    public:
        static StringId generate(const char* str, std::size_t len) { return StringId{str, len}; }
        static StringId generate(std::string_view str) { return generate(str.data(), str.size()); }
        bool operator==(const StringId& other) const { return m_sid == other.m_sid; }
        std::string_view getString() const { return m_str; }
        StringIdType getSid() const { return m_sid; }

    private:
        StringId(std::string_view str, std::size_t len)
            : m_str{str}, m_sid{Hash::hash32(str.data(), len)}
        {
        }
        std::string m_str{};
        StringIdType m_sid{};
    };

    /**
     * User-defined literal that transforms the syntax "any_string"_sid into a StringId object at compile
     * time.
     */
    constexpr StringId operator""_sid(const char* str, std::size_t len)
    {
        return StringId::generate(str, len);
    }

    /**
     * Macro that transforms SID("any_string") into "any_string"_sid.
     */
#define SID(str) str##_sid
} // namespace Engine

template <>
struct std::hash<Engine::StringId> {
    std::size_t operator()(const Engine::StringId& sid) const noexcept { return sid.getSid(); }
};

#endif
