#ifndef HASH_H
#define HASH_H

#include <cstdint>
#include <wyhash/wyhash32.h>

namespace Engine::Hash
{
    constexpr std::uint32_t hash32(const char* str, std::size_t len) { return wyhash32(str, len, 0); }
} // namespace Engine::Hash

#endif
