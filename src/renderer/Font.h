#ifndef FONT_H
#define FONT_H

#include <SDL_ttf.h>

namespace Engine
{
    class Font
    {
    public:
        Font(TTF_Font* font)
            : m_data{font}
        {
        }
        Font(const Font&) = delete;
        Font(Font&&) = delete;
        Font& operator=(const Font&) = delete;
        Font& operator=(Font&&) = delete;
        ~Font();
        TTF_Font* getData() const { return m_data; }

    private:
        TTF_Font* m_data{};
    };
} // namespace Engine

#endif
