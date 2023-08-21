#include "../../include/Container/flags.h"

bool operator==(const Flags& Left, const Flags& Right )
{
    return (Left.mFlags == Right.mFlags);
}

bool operator!=(const Flags& Left, const Flags& Right )
{
    return (Left.mFlags != Right.mFlags);
}

Flags operator|(const Flags& Left, const Flags& Right )
{
    Flags results(Left);
    results |= Right;
    return results;
}

Flags operator&(const Flags& Left, const Flags& Right )
{
    Flags results(Left);
    results &= Right;
    return results;
}

const Flags& Flags::operator|=(const Flags& Other )
{
    mIsDefined |= Other.mIsDefined;
    mFlags |= Other.mFlags;
    return *this;
}

const Flags& Flags::operator&=(const Flags& Other )
{
    mIsDefined |= Other.mIsDefined;
    mFlags &= Other.mFlags;
    return *this;
}