#ifndef ASSERTION_H
#define ASSERTION_H

#define STATIC_ASSERT(expr) static_assert(expr, #expr)

#if ASSERTIONS_ENABLED

#include <cstdio>

#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#elif defined(__clang__)
#define DEBUG_BREAK() __builtin_debugtrap()
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#define DEBUG_BREAK() __asm__ volatile("int $0x03")
#elif defined(__GNUC__) && defined(__thumb__)
#define DEBUG_BREAK() __asm__ volatile(".inst 0xde01")
#elif defined(__GNUC__) && defined(__arm__) && !defined(__thumb__)
#define DEBUG_BREAK() __asm__ volatile(".inst 0xe7f001f0")
#else
#error "Could not define DEBUG_BREAK()"
#endif

#define REPORT_ASSERTION_FAILURE(expr, file, line)                                                           \
    printf("Assertion failed at %s (line %d): %s\n", file, line, expr)

#define ASSERT(expr)                                                                                         \
    if (expr) {                                                                                              \
    } else {                                                                                                 \
        REPORT_ASSERTION_FAILURE(#expr, __FILE__, __LINE__);                                                 \
        DEBUG_BREAK();                                                                                       \
    }

#else

#define ASSERT(expr)

#endif

#endif
