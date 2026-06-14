#include "StringId.h"

#include "Hash.h"
#include "Locator.h"

#include <unordered_map>

namespace Engine
{
    StringId StringId::Intern(std::string_view str)
    {
        const StringIdType id{Hash::Hash32(str.data())};
        if (!s_stringIdTable.contains(id)) {
            s_stringIdTable[id] = std::string{str};
            Locator::GetLogger()->Debug("[StringId] Interned '{}' as '{}'", str, id);
        }
        return StringId{s_stringIdTable[id], id};
    }

    std::string_view StringId::GetString(StringIdType id)
    {
        if (s_stringIdTable.contains(id)) {
            return s_stringIdTable[id];
        }
        return {};
    }

    StringId::StringId(std::string_view str, StringIdType id)
        : m_str{str}, m_id{id}
    {
    }
} // namespace Engine
