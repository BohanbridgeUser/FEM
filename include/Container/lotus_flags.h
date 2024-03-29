#ifndef _LOTUS_FLAGS_H_
#define _LOTUS_FLAGS_H_
#include "../define.h"
#include "flags.h"

LOTUS_CREATE_FLAG(STRUCTURE,       63);
LOTUS_CREATE_FLAG(FLUID,           62);
LOTUS_CREATE_FLAG(THERMAL,         61);
LOTUS_CREATE_FLAG(VISITED,         60);
LOTUS_CREATE_FLAG(SELECTED,        59);
LOTUS_CREATE_FLAG(BOUNDARY,        58);
LOTUS_CREATE_FLAG(INLET,           57);
LOTUS_CREATE_FLAG(OUTLET,          56);
LOTUS_CREATE_FLAG(SLIP,            55);
LOTUS_CREATE_FLAG(INTERFACE,       54);
LOTUS_CREATE_FLAG(CONTACT,         53);
LOTUS_CREATE_FLAG(TO_SPLIT,        52);
LOTUS_CREATE_FLAG(TO_ERASE,        51);
LOTUS_CREATE_FLAG(TO_REFINE,       50);
LOTUS_CREATE_FLAG(NEW_ENTITY,      49);
LOTUS_CREATE_FLAG(OLD_ENTITY,      48);
LOTUS_CREATE_FLAG(ACTIVE,          47);
LOTUS_CREATE_FLAG(MODIFIED,        46);
LOTUS_CREATE_FLAG(RIGID,           45);
LOTUS_CREATE_FLAG(SOLID,           44);
LOTUS_CREATE_FLAG(MPI_BOUNDARY,    43);
LOTUS_CREATE_FLAG(INTERACTION,     42);
LOTUS_CREATE_FLAG(ISOLATED,        41);
LOTUS_CREATE_FLAG(MASTER,          40);
LOTUS_CREATE_FLAG(SLAVE,           39);
LOTUS_CREATE_FLAG(INSIDE,          38);
LOTUS_CREATE_FLAG(FREE_SURFACE,    37);
LOTUS_CREATE_FLAG(BLOCKED,         36);
LOTUS_CREATE_FLAG(MARKER,          35);
LOTUS_CREATE_FLAG(PERIODIC,        34);
LOTUS_CREATE_FLAG(WALL,            33);

const Flags ALL_DEFINED(Flags::AllDefined());
const Flags ALL_TRUE(Flags::AllTrue());

#endif
