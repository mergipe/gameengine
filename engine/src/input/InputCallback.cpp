#include "InputCallback.h"

namespace Engine
{
    const InputCallback* InputCallbackMapping::GetCommandCallback(const StringId& commandId) const
    {
        const auto it{m_mapping.find(commandId)};
        if (it != m_mapping.end()) {
            return &it->second;
        }
        return nullptr;
    }

    void InputCallbackMapping::SetCommandCallback(const StringId& commandId, const InputCallback& callback)
    {
        m_mapping[commandId] = callback;
    }
} // namespace Engine
