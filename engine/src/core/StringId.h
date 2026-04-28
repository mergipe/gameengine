#ifndef STRING_ID_H
#define STRING_ID_H

#include "Hash.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Engine
{
    using StringIdType = std::uint32_t;

    /**
     * Stores interned strings mapped by its hash values.
     */
    class StringIdTable
    {
    public:
        StringIdTable() = delete;
        static StringIdType InternString(std::string_view sv);
        static std::string_view GetString(StringIdType id);

    private:
        static inline std::unordered_map<StringIdType, std::string> s_table{};
    };

    /**
     * Wraps a view to an interned string and its hashed value.
     */
    class StringId
    {
    public:
        StringId() = default;
        explicit StringId(std::string_view sv)
            : m_sid{StringIdTable::InternString(sv)}
        {
            m_str = StringIdTable::GetString(m_sid);
        }
        bool operator==(const StringId& other) const { return m_sid == other.m_sid; }
        std::string_view GetString() const { return m_str; }
        StringIdType GetSid() const { return m_sid; }

    private:
        std::string_view m_str{};
        StringIdType m_sid{};
    };
} // namespace Engine

template <>
struct std::hash<Engine::StringId> {
    std::size_t operator()(const Engine::StringId& sid) const noexcept { return sid.GetSid(); }
};

#endif
