#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

namespace Engine
{
    using U8 = std::uint8_t;
    using U16 = std::uint16_t;
    using U32 = std::uint32_t;
    using U64 = std::uint64_t;

    struct RGBA8 {
        static RGBA8 FromHex(int colorHex, float alpha = 1.0f);
        U8 r{};
        U8 g{};
        U8 b{};
        U8 a{};
    };
} // namespace Engine

#endif
