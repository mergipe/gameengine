#pragma once

#include "Hash.h"
#include "Types.h"

#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Engine
{
    using StringIdType = U32;

    constexpr U32 operator""_sid(const char* str, std::size_t) { return Hash::Hash32(str); }

    /**
     * Wraps a view to an interned string and its hashed value.
     */
    class StringId
    {
    public:
        static StringId Intern(std::string_view str);
        static std::string_view GetString(StringIdType id);

        StringId() = default;
        constexpr explicit StringId(const char* str);

        bool operator==(const StringId& other) const { return m_id == other.m_id; }
        [[nodiscard]] std::string_view GetString() const { return m_str; }
        [[nodiscard]] StringIdType GetId() const { return m_id; }

    private:
        static inline std::unordered_map<StringIdType, std::string> s_stringIdTable{};

        explicit StringId(std::string_view str, StringIdType id);

        std::string_view m_str{};
        StringIdType m_id{};
    };

    constexpr StringId::StringId(const char* str)
        : m_str{str}, m_id{Hash::Hash32(str)}
    {
    }

#define SID(str)                                                                                             \
    StringId { str }
} // namespace Engine

template <>
struct std::hash<Engine::StringId> {
    std::size_t operator()(const Engine::StringId& sid) const noexcept { return sid.GetId(); }
};
