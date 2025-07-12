#include "StringId.h"
#include "Hash.h"
#include "Locator.h"
#include <unordered_map>

namespace Engine
{
    StringIdType StringIdTable::internString(std::string_view sv)
    {
        const StringIdType sid{Hash::hash32(sv.data(), sv.size())};
        if (!s_table.contains(sid)) {
            s_table[sid] = std::string{sv};
            Locator::getLogger()->debug("[StringId] Interned '{}' as '{}'", sv, sid);
        }
        return sid;
    }

    std::string_view StringIdTable::getString(StringIdType id)
    {
        const auto it{s_table.find(id)};
        if (it != s_table.end()) {
            return it->second;
        }
        return {};
    }
} // namespace Engine
