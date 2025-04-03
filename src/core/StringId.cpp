#include "StringId.h"
#include "Logger.h"
#include <unordered_map>

namespace Engine
{
    static std::unordered_map<StringIdType, std::string> g_stringIdTable;

    StringId internString(std::string_view str)
    {
        const StringIdType id{hash(str, str.length())};
        if (!g_stringIdTable.contains(id)) {
            g_stringIdTable[id] = std::string{str};
            Logger::debug("Interned ('{}', SID={})", str, id);
        } else if (str != g_stringIdTable[id]) {
            Logger::error("Hash collision (SID={}) between '{}' and '{}'", id, str.data(),
                          g_stringIdTable[id].data());
            return StringId{str, id};
        }
        return StringId{g_stringIdTable[id], id};
    }
} // namespace Engine
