#include "Types.h"

namespace Engine
{
    RGBA8 RGBA8::FromHex(int colorHex, float alpha)
    {
        return {static_cast<U8>((colorHex >> 16) & 0xFF), static_cast<U8>((colorHex >> 8) & 0xFF),
                static_cast<U8>(colorHex & 0xFF), static_cast<U8>(0xFF * alpha)};
    }
} // namespace Engine
