#include "StringId.h"
#include "Logger.h"
#include <unordered_map>

namespace Engine
{
    static std::unordered_map<StringIdType, std::string> g_stringIdTable;

    StringId internString(std::string_view str)
    {
        StringId sid{hash(str, str.length())};
        if (!g_stringIdTable.contains(sid.id)) {
            g_stringIdTable[sid.id] = std::string{str};
            Logger::debug("Interned ('{}', SID={})", str, sid.id);
        } else if (str != g_stringIdTable[sid.id]) {
            Logger::error("Hash collision (SID={}) between {} and {}", sid.id, str.data(),
                          g_stringIdTable[sid.id].data());
        }
        return sid;
    }
} // namespace Engine
