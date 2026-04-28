#ifndef FONT_H
#define FONT_H

namespace Engine
{
    class Font
    {
    public:
        Font() = default;
        Font(const Font&) = delete;
        Font(Font&&) = delete;
        Font& operator=(const Font&) = delete;
        Font& operator=(Font&&) = delete;
        ~Font() = default;
    };
} // namespace Engine

#endif
