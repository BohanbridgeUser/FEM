// System includes
#include <string>

// External includes

// Project includes
#include "../include/lotus_version.h"

// LOTUS patch
#ifndef LOTUS_PATCH_VERSION
#define LOTUS_PATCH_VERSION "0"
#endif

// GiT revision at configure
#ifndef LOTUS_SHA1_NUMBER
#define LOTUS_SHA1_NUMBER "0"
#endif

// GiT branch name at configure
#ifndef LOTUS_BRANCH_NAME
#define LOTUS_BRANCH_NAME ""
#endif

// Build type
#ifndef LOTUS_BUILD_TYPE
#define LOTUS_BUILD_TYPE "Release"
#endif

// Architecture type
#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64)
#define LOTUS_ARCH_TYPE "x86_64"
#elif defined(__i386__) || defined(_M_IX86) || defined(_X86_) || defined(__i386)
#define LOTUS_ARCH_TYPE "x86"
#elif defined(__arm__)
#define LOTUS_ARCH_TYPE "ARM32"
#elif defined(__aarch64__)
#define LOTUS_ARCH_TYPE "ARM64"
#else
#define LOTUS_ARCH_TYPE "Unknown architecture"
#endif

// Full version
#ifndef LOTUS_TO_STRING_
    #define LOTUS_TO_STRING_(X) #X
#endif
#ifndef LOTUS_TO_STRING
    #define LOTUS_TO_STRING(X) LOTUS_TO_STRING_(X)
#endif
#define LOTUS_VERSION_STRING \
LOTUS_TO_STRING(LOTUS_MAJOR_VERSION) "." \
LOTUS_TO_STRING(LOTUS_MINOR_VERSION) "." \
LOTUS_TO_STRING(LOTUS_PATCH_VERSION) "-" \
LOTUS_BRANCH_NAME "-" \
LOTUS_SHA1_NUMBER "-" \
LOTUS_BUILD_TYPE  "-" \
LOTUS_ARCH_TYPE

// Define OS name
#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    #define LOTUS_OS_NAME "GNU/Linux"
#elif defined(__APPLE__) && defined(__MACH__)
    #define LOTUS_OS_NAME "Mac OS"
#elif defined(_WIN32) || defined(_WIN64)
    #define LOTUS_OS_NAME "Windows" 
#else
    #define LOTUS_OS_NAME "Unknown OS"
#endif

// Define compiler label
#if defined(__clang__)
    #define LOTUS_COMPILER_LABEL "Clang-" \
    LOTUS_TO_STRING(__clang_major__) \
    "." \
    LOTUS_TO_STRING(__clang_minor__)
#elif defined(__GNUC__) || defined(__GNUG__)
    #define LOTUS_COMPILER_LABEL "GCC-" \
    LOTUS_TO_STRING(__GNUC__) \
    "." \
    LOTUS_TO_STRING(__GNUC_MINOR__)
#elif defined(_MSC_VER)
    #define LOTUS_COMPILER_LABEL "MSVC-" \
    LOTUS_TO_STRING(_MSC_VER)
#else
    #define LOTUS_COMPILER_LABEL "Unknown compiler"
#endif

std::string GetPatchVersion() 
{
    return LOTUS_PATCH_VERSION;
}
std::string GetCommit() 
{
    return LOTUS_SHA1_NUMBER;
}
std::string GetBranchName() 
{
    return LOTUS_BRANCH_NAME;
}
std::string GetBuildType() 
{
    return LOTUS_BUILD_TYPE;
}
std::string GetVersionString() 
{
    return LOTUS_VERSION_STRING;
}
std::string GetOSName() 
{
    return LOTUS_OS_NAME;
}

std::string GetCompiler() 
{
    return LOTUS_COMPILER_LABEL;
}