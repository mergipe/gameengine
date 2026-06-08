#pragma once

#include "Types.h"

namespace Engine::Hash
{
    constexpr U32 g_hashInit{0x811c9dc5};

    // FNV-1a
    constexpr U32 Hash32(const char* str, U32 hash)
    {
        while (*str) {
            hash ^= static_cast<U8>(*str++);
            hash *= 0x01000193;
        }
        return hash;
    }

    constexpr U32 Hash32(const char* str) { return Hash32(str, g_hashInit); }
} // namespace Engine::Hash
