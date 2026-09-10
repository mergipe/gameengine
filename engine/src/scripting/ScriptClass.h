#pragma once

#include "core/StringId.h"
#include "core/Variant.h"

#include <filesystem>
#include <sol/sol.hpp>
#include <string>
#include <string_view>

namespace Engine
{
    class Entity;
    class ScriptInstance;

    struct ScriptClassData final {
        StringId classId{};
        std::unordered_map<std::string, Variant> attributes{};
    };

    class ScriptClass
    {
    public:
        ScriptClass(const StringId& id, std::string_view name, const sol::table& luaTable);
        [[nodiscard]] const StringId& GetId() const;
        [[nodiscard]] std::string_view GetName() const;
        [[nodiscard]] const void* GetLuaTablePointer() const;
        std::optional<ScriptInstance> ConstructInstance(Entity& entity);

    private:
        std::string m_name{};
        StringId m_id{};
        sol::table m_luaTable{};
    };
} // namespace Engine
