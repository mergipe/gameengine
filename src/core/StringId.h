#ifndef STRINGID_H
#define STRINGID_H

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
        static StringIdType internString(std::string_view sv);
        static std::string_view getString(StringIdType id);

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
            : m_sid{StringIdTable::internString(sv)}
        {
            m_str = StringIdTable::getString(m_sid);
        }
        bool operator==(const StringId& other) const { return m_sid == other.m_sid; }
        std::string_view getString() const { return m_str; }
        StringIdType getSid() const { return m_sid; }

    private:
        std::string_view m_str{};
        StringIdType m_sid{};
    };
} // namespace Engine

template <>
struct std::hash<Engine::StringId> {
    std::size_t operator()(const Engine::StringId& sid) const noexcept { return sid.getSid(); }
};

#endif
