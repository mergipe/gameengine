#include "StringId.h"

#include "Hash.h"
#include "Locator.h"

#include <unordered_map>

namespace Engine
{
    StringIdType StringIdTable::InternString(std::string_view sv)
    {
        const StringIdType sid{Hash::Hash32(sv.data(), sv.size())};
        if (!s_table.contains(sid)) {
            s_table[sid] = std::string{sv};
            Locator::GetLogger()->Debug("[StringId] Interned '{}' as '{}'", sv, sid);
        }
        return sid;
    }

    std::string_view StringIdTable::GetString(StringIdType id)
    {
        const auto it{s_table.find(id)};
        if (it != s_table.end()) {
            return it->second;
        }
        return {};
    }
} // namespace Engine
