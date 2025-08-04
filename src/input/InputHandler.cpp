#include "InputHandler.h"
#include "InputConfigLoader.h"
#include "InputValue.h"
#include "core/Assert.h"
#include "core/Locator.h"
#include "game/Game.h"
#include <SDL3/SDL.h>
#include <functional>
#include <oneapi/tbb/detail/_template_helpers.h>
#include <utility>

namespace Engine
{
    InputHandler::InputHandler(const std::filesystem::path& inputConfigFilepath)
    {
        m_inputConfig = InputConfigLoader::load(inputConfigFilepath);
        StringId emptyScopeId{"empty"};
        m_inputConfig.inputScopes.insert(std::make_pair(emptyScopeId, InputScope{emptyScopeId}));
        if (Game::instance().hasDevMode()) {
            m_inputConfig.inputScopes.insert(std::make_pair(m_devGuiScopeId, InputScope{m_devGuiScopeId}));
        }
        m_currentScope = &m_inputConfig.inputScopes.at(emptyScopeId);
        m_previousScope = m_currentScope;
        m_engineCallbackMapping.setCommandCallback(
            StringId{"toggle_dev_mode"},
            InputCallback{[](const InputValue&) { Game::instance().toggleDevMode(); }});
        // for now only keyboard support
        if (SDL_HasKeyboard()) {
            // from what I know, SDL_KeyboardEvent.which is always 0 (unknown/virtual keyboard)
            m_inputDevices.emplace_back(InputDevice{InputDevice::Type::keyboard, 0});
        }
    }

    const StringId& InputHandler::getCurrentScopeId() const
    {
        ASSERT(m_currentScope != nullptr);
        return m_currentScope->getId();
    }

    const StringId& InputHandler::getPreviousScopeId() const
    {
        ASSERT(m_previousScope != nullptr);
        return m_previousScope->getId();
    }

    void InputHandler::switchScope(const StringId& scopeId)
    {
        if (m_currentScope == nullptr || m_currentScope->getId() != scopeId) {
            const auto it{m_inputConfig.inputScopes.find(scopeId)};
            if (it != m_inputConfig.inputScopes.end()) {
                m_previousScope = m_currentScope;
                m_currentScope = &it->second;
            }
        }
    }

    void InputHandler::handleKeyboardKeyDownEvent(const SDL_KeyboardEvent& event)
    {
        if (event.repeat) {
            return;
        }
        if (auto* command{findCommand(InputDevice::Type::keyboard, event.scancode)}) {
            handleControlDown(*command, InputDevice::Id{InputDevice::Type::keyboard, event.which});
        }
    }

    void InputHandler::handleKeyboardKeyUpEvent(const SDL_KeyboardEvent& event)
    {
        if (auto* command{findCommand(InputDevice::Type::keyboard, event.scancode)}) {
            handleControlUp(*command, InputDevice::Id{InputDevice::Type::keyboard, event.which});
        }
    }

    void InputHandler::resolveInput()
    {
        for (std::size_t i{0}; i < m_inputEventsCount; ++i) {
            auto& inputEvent{m_unhandledInputEvents[i]};
            if (inputEvent.isEngineCommand) {
                const InputCallback* callback{
                    m_engineCallbackMapping.getCommandCallback(inputEvent.commandId)};
                if (callback) {
                    callback->execute(inputEvent.inputValue);
                }
            } else {
                Locator::getEventBus()->dispatchEvent(std::move(inputEvent));
            }
        }
        m_inputEventsCount = 0;
    }

    const InputDevice::Id& InputHandler::acquireAvailableDevice()
    {
        // for now only return the keyboard
        InputDevice& device{m_inputDevices.front()};
        device.inUse = true;
        return device.id;
    }

    InputCommand* InputHandler::findCommand(InputDevice::Type deviceType, int controlCode)
    {
        ASSERT(m_currentScope != nullptr);
        InputCommand* command{m_currentScope->findCommand(deviceType, controlCode)};
        if (!command) {
            command = m_inputConfig.engineInputMapping.findCommand(deviceType, controlCode);
        }
        return command;
    }

    void InputHandler::triggerCommand(const InputCommand& command, const InputDevice::Id& deviceId,
                                      const InputValue& inputValue)
    {
        m_unhandledInputEvents[m_inputEventsCount++] =
            InputEvent{command.getName(), deviceId, inputValue, command.isEngineCommand()};
    }

    void InputHandler::handleControlDown(const InputCommand& command, const InputDevice::Id& deviceId)
    {
        if (command.getType() == InputCommand::Type::control_down ||
            command.getType() == InputCommand::Type::control_state) {
            triggerCommand(command, deviceId, InputValue{1});
        }
    }

    void InputHandler::handleControlUp(const InputCommand& command, const InputDevice::Id& deviceId)
    {
        if (command.getType() == InputCommand::Type::control_state) {
            triggerCommand(command, deviceId, InputValue{0});
        }
    }
} // namespace Engine
