#pragma once

#include "ScriptClass.h"
#include "core/FileSystem.h"
#include "core/Variant.h"
#include "scene/Entity.h"

#include <entt/entity/handle.hpp>
#include <filesystem>
#include <optional>

namespace Engine
{
    struct ScriptData final {
        std::filesystem::path filePath{};
        std::string className{};
        std::unordered_map<std::string, Variant> attributes{};
    };

    class ScriptInstance;

    class ScriptSystem
    {
    public:
        void Init();
        void ShutDown();
        std::optional<ScriptInstance> CreateScriptInstance(const ScriptData& scriptData,
                                                           entt::handle entityHandle);

    private:
        static inline const std::filesystem::path s_scriptingLibPath{
            FileSystem::GetAbsolutePath("scripting")};

        void AppendPackagePath(const std::string& packagePath);
        bool LuaInstanceOf(const sol::table& lhs, const sol::table& rhs);
        ScriptClass* GetOrLoadScriptClass(const std::filesystem::path& filePath, std::string_view className);
        sol::object GetComponent(Entity entity, const sol::table& type);
        ScriptClass* GetScriptClass(const StringId& scriptId) const;
        void SetBindings();
        void BindCoreTypes();
        void BindComponentTypes();

        std::unordered_map<StringId, std::unique_ptr<ScriptClass>> m_scriptClasses{};
        std::unordered_map<const void*, std::function<sol::object(Entity)>> m_componentTypes{};
        sol::state m_lua{};
    };
} // namespace Engine
