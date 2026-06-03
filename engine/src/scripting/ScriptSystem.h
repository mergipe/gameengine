#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "ScriptClass.h"
#include "ScriptInstance.h"
#include "core/FileSystem.h"
#include "scene/Entity.h"

#include <entt/entity/handle.hpp>
#include <filesystem>
#include <optional>

namespace Engine
{
    class ScriptSystem
    {
    public:
        std::optional<ScriptInstance> CreateScriptInstance(const std::filesystem::path& filePath,
                                                           std::string_view className,
                                                           entt::handle entityHandle);
        void Init();
        void ShutDown();

    private:
        static inline const std::filesystem::path s_scriptingLibPath{FileSystem::GetAbsolutePath("lua")};

        std::unique_ptr<ScriptClass> LoadScriptClass(const std::filesystem::path& filePath,
                                                     std::string_view className);
        void CreateScriptBindings();
        void StoreScriptClass(const StringId& scriptId, std::unique_ptr<ScriptClass> scriptClass);
        sol::object GetComponent(Entity entity, const sol::table& type);
        ScriptClass* GetScriptClass(const StringId& scriptId) const;

        std::unordered_map<StringId, std::unique_ptr<ScriptClass>> m_scriptClasses{};
        std::unordered_map<const void*, std::function<sol::object(Entity)>> m_componentTypes{};
        sol::state m_lua{};
    };
} // namespace Engine

#endif
