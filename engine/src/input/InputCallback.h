#ifndef INPUT_CALLBACK_H
#define INPUT_CALLBACK_H

#include "InputValue.h"
#include "core/StringId.h"

#include <functional>
#include <unordered_map>

namespace Engine
{
    class InputCallback
    {
    public:
        InputCallback() = default;
        explicit InputCallback(const std::function<void(const InputValue&)>& function)
            : m_function{function}
        {
        }
        void Execute(const InputValue& value) const { m_function(value); }

    private:
        std::function<void(const InputValue&)> m_function{[](const InputValue&) {}};
    };

    class InputCallbackMapping
    {
    public:
        InputCallbackMapping() = default;
        explicit InputCallbackMapping(const std::unordered_map<StringId, InputCallback>& mapping)
            : m_mapping{mapping}
        {
        }
        const InputCallback* GetCommandCallback(const StringId& commandId) const;
        void SetCommandCallback(const StringId& commandId, const InputCallback& callback);

    private:
        std::unordered_map<StringId, InputCallback> m_mapping{};
    };
} // namespace Engine

#endif
