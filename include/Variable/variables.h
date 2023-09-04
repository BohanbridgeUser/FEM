#ifndef _VARIABLES_H_
#define _VARIABLES_H_
#include "../define.h"
#include "variable.h"

LOTUS_DEFINE_3D_VARIABLES_WITH_COMPONENTS(double, DISPLACEMENT)
LOTUS_DEFINE_3D_VARIABLES_WITH_COMPONENTS(double, VELOCITY)
LOTUS_DEFINE_3D_VARIABLES_WITH_COMPONENTS(double, ROTATION)
LOTUS_DEFINE_3D_VARIABLES_WITH_COMPONENTS(double, ACCELERATION)
LOTUS_DEFINE_3D_VARIABLES_WITH_COMPONENTS(double, ANGULAR_ACCELERATION)
LOTUS_DEFINE_3D_VARIABLES_WITH_COMPONENTS(double, EXTERNAL_FORCE )
LOTUS_DEFINE_3D_VARIABLES_WITH_COMPONENTS(double, EXTERNAL_MOMENT )
LOTUS_DEFINE_3D_VARIABLES_WITH_COMPONENTS(double, FORCE_RESIDUAL )
LOTUS_DEFINE_3D_VARIABLES_WITH_COMPONENTS(double, MOMENT_RESIDUAL )

typedef std::array<double,3> 
                                                    Array;
LOTUS_DEFINE_VARIABLE(Array, EXTERNAL_FORCES_VECTOR)
LOTUS_DEFINE_VARIABLE(Array, RESIDUAL_VECTOR)
LOTUS_DEFINE_VARIABLE(Array, EXTERNAL_ENERGY)

#endif