#pragma once
#include <iostream>
#include <assert.h>
#include <GL/glew.h>

#include "colormod.h"

#if defined(_MSC_VER)  // Microsoft Visual C++
    #include <intrin.h>
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__i386__) || defined(__x86_64__)
    // Use inline assembly for x86/x86_64
    #define DEBUG_BREAK() __asm__ volatile("int3")
#else
    // Fallback on non-x86 platforms
    #include <signal.h>
    #define DEBUG_BREAK() raise(SIGTRAP)
#endif

// ASSERT macro that shows file, line, and the failed expression
#define ASSERT(x)                                                      \
    do {                                                               \
        if (!(x)) {                                                    \
            std::cerr << "Assertion Failed: " << #x << '\n'           \
                      << "File: " << __FILE__ << '\n'                  \
                      << "Line: " << __LINE__ << std::endl;            \
            DEBUG_BREAK();                                             \
        }                                                              \
    } while (false)

#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall())

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

// Declare global functions
void GLClearError();
bool GLLogCall();

