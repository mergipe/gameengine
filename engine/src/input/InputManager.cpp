#include "InputManager.h"

#include "Engine.h"
#include "InputConfigLoader.h"
#include "InputValue.h"
#include "core/Assert.h"
#include "core/Locator.h"

#include <SDL3/SDL.h>
#include <functional>
#include <utility>

namespace Engine
{
    void InputManager::Init()
    {
        const std::filesystem::path inputConfigFilePath{
            ResourceManager::GetResourceAbsolutePath("input.config")};
        m_inputConfig = InputConfigLoader::Load(inputConfigFilePath);
        StringId emptyScopeId{SID("empty")};
        m_inputConfig.inputScopes.insert(std::make_pair(emptyScopeId, InputScope{emptyScopeId}));
        if (Engine::Instance().HasDevMode()) {
            m_inputConfig.inputScopes.insert(std::make_pair(s_devGuiScopeId, InputScope{s_devGuiScopeId}));
        }
        m_currentScope = &m_inputConfig.inputScopes.at(emptyScopeId);
        m_previousScope = m_currentScope;
        m_engineCallbackMapping.SetCommandCallback(
            SID("toggle_dev_mode"),
            InputCallback{[](const InputValue&) { Engine::Instance().ToggleDevMode(); }});
        // for now only keyboard support
        if (SDL_HasKeyboard()) {
            // the keyboard id will always be 0 because of x11 and wayland differences
            m_inputDevices.emplace_back(InputDevice{InputDevice::Type::keyboard, 0});
        }
        Locator::GetLogger()->Info("Input manager initialized");
    }

    void InputManager::ShutDown() { Locator::GetLogger()->Info("Input manager shut down"); }

    const StringId& InputManager::GetCurrentScopeId() const
    {
        ASSERT(m_currentScope != nullptr);
        return m_currentScope->GetId();
    }

    const StringId& InputManager::GetPreviousScopeId() const
    {
        ASSERT(m_previousScope != nullptr);
        return m_previousScope->GetId();
    }

    void InputManager::SwitchScope(const StringId& scopeId)
    {
        if (m_currentScope == nullptr || m_currentScope->GetId() != scopeId) {
            const auto it{m_inputConfig.inputScopes.find(scopeId)};
            if (it != m_inputConfig.inputScopes.end()) {
                m_previousScope = m_currentScope;
                m_currentScope = &it->second;
            }
        }
    }

    void InputManager::HandleKeyboardKeyDownEvent(const SDL_KeyboardEvent& event)
    {
        if (event.repeat) {
            return;
        }
        if (auto* command{FindCommand(InputDevice::Type::keyboard, event.scancode)}) {
            HandleControlDown(*command, InputDevice::Id{InputDevice::Type::keyboard, 0});
        }
    }

    void InputManager::HandleKeyboardKeyUpEvent(const SDL_KeyboardEvent& event)
    {
        if (auto* command{FindCommand(InputDevice::Type::keyboard, event.scancode)}) {
            HandleControlUp(*command, InputDevice::Id{InputDevice::Type::keyboard, 0});
        }
    }

    void InputManager::ResolveInput()
    {
        for (std::size_t i{0}; i < m_inputEventsCount; ++i) {
            auto& inputEvent{m_unhandledInputEvents[i]};
            if (inputEvent.isEngineCommand) {
                const InputCallback* callback{
                    m_engineCallbackMapping.GetCommandCallback(inputEvent.commandId)};
                if (callback) {
                    callback->Execute(inputEvent.inputValue);
                }
            } else {
                Locator::GetEventBus()->DispatchEvent(std::move(inputEvent));
            }
        }
        m_inputEventsCount = 0;
    }

    const InputDevice::Id& InputManager::AcquireAvailableDevice()
    {
        // for now only return the keyboard
        InputDevice& device{m_inputDevices.front()};
        device.inUse = true;
        return device.id;
    }

    InputCommand* InputManager::FindCommand(InputDevice::Type deviceType, int controlCode)
    {
        ASSERT(m_currentScope != nullptr);
        InputCommand* command{m_currentScope->FindCommand(deviceType, controlCode)};
        if (!command) {
            command = m_inputConfig.engineInputMapping.FindCommand(deviceType, controlCode);
        }
        return command;
    }

    void InputManager::TriggerCommand(const InputCommand& command, const InputDevice::Id& deviceId,
                                      const InputValue& inputValue)
    {
        m_unhandledInputEvents[m_inputEventsCount++] =
            InputEvent{command.GetName(), deviceId, inputValue, command.IsEngineCommand()};
    }

    void InputManager::HandleControlDown(const InputCommand& command, const InputDevice::Id& deviceId)
    {
        if (command.GetType() == InputCommand::Type::control_down ||
            command.GetType() == InputCommand::Type::control_state) {
            TriggerCommand(command, deviceId, InputValue{1});
        }
    }

    void InputManager::HandleControlUp(const InputCommand& command, const InputDevice::Id& deviceId)
    {
        if (command.GetType() == InputCommand::Type::control_state) {
            TriggerCommand(command, deviceId, InputValue{0});
        }
    }
} // namespace Engine
