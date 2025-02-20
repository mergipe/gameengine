#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

namespace Engine
{
    struct Color {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a;
    };
} // namespace Engine

#endif
