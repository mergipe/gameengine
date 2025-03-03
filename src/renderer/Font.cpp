#include "Font.h"

namespace Engine
{
    Font::~Font()
    {
        TTF_CloseFont(m_data);
        m_data = nullptr;
    }
} // namespace Engine
