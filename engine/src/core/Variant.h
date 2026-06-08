#pragma once

#include "StringId.h"

namespace Engine
{
    struct Variant {
        enum class Type { tInteger, tFloat, tBool, tStringId, tCount };
        Type type{};
        union {
            int asInteger;
            float asFloat;
            bool asBool;
            StringIdType asStringId;
        };
    };
} // namespace Engine
