#ifndef _REGISTER_LOTUS_COMPONENTS_FOR_GEOMETRY_H_
#define _REGISTER_LOTUS_COMPONENTS_FOR_GEOMETRY_H_

#include "geometry.h"
#include "../Node/node.h"

extern template class Lotus_Components<Geometry<Node>>;

void AddComponent(std::string const& Name, Geometry<Node> const& ThisComponent);

#endif
