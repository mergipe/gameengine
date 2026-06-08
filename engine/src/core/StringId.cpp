#include "StringId.h"

#include "Hash.h"
#include "Locator.h"

#include <unordered_map>

namespace Engine
{
    StringId StringId::Intern(std::string_view str)
    {
        const StringIdType sid{Hash::Hash32(str.data())};
        if (!s_stringIdTable.contains(sid)) {
            s_stringIdTable[sid] = std::string{str};
            Locator::GetLogger()->Debug("[StringId] Interned '{}' as '{}'", str, sid);
        }
        return StringId{s_stringIdTable[sid], sid};
    }

    std::string_view StringId::GetString(StringIdType sid)
    {
        if (s_stringIdTable.contains(sid)) {
            return s_stringIdTable[sid];
        }
        return {};
    }

    StringId::StringId(std::string_view str, StringIdType sid)
        : m_str{str}, m_sid{sid}
    {
    }
} // namespace Engine
