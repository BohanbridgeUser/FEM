#include <string>
#pragma once

#ifndef LOTUS_MAJOR_VERSION
#define LOTUS_MAJOR_VERSION 9
#endif

#ifndef LOTUS_MINOR_VERSION
#define LOTUS_MINOR_VERSION 0
#endif

constexpr int GetMajorVersion() 
{
    return LOTUS_MAJOR_VERSION;
}

constexpr int GetMinorVersion() 
{
    return LOTUS_MINOR_VERSION;
}

std::string GetPatchVersion();
std::string GetCommit();
std::string GetBranchName();
std::string GetBuildType();
std::string GetVersionString();
std::string GetOSName();
std::string GetCompiler();