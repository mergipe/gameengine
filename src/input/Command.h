#ifndef COMMAND_H
#define COMMAND_H

#include <functional>

class Command
{
public:
    Command(const std::function<void()>& function, bool allowRepeat)
        : m_function{function}, m_allowRepeat{allowRepeat}
    {
    }
    bool allowRepeat() const { return m_allowRepeat; }
    void execute() const { m_function(); }

private:
    std::function<void()> m_function{};
    bool m_allowRepeat{};
};

#endif
