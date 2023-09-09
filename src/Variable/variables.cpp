#include "../../include/Variable/variables.h"
#include "../../include/Constitutive_Law/Constitutive_law.h"

LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(DISPLACEMENT)
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(VELOCITY)
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(ROTATION)
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(ACCELERATION)
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(ANGULAR_ACCELERATION)
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(VOLUME_ACCELERATION)
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(EXTERNAL_FORCE)
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(EXTERNAL_MOMENT)
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(FORCE_RESIDUAL )
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(MOMENT_RESIDUAL )
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(INTERNAL_FORCE)
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(FORCE_LOAD)
LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(POINT_LOAD)


LOTUS_CREATE_VARIABLE(Array, EXTERNAL_FORCES_VECTOR)
LOTUS_CREATE_VARIABLE(Array, RESIDUAL_VECTOR)
LOTUS_CREATE_VARIABLE(Array, EXTERNAL_ENERGY)
LOTUS_CREATE_VARIABLE(Array, FORCE_LOAD_VECTOR)

LOTUS_CREATE_VARIABLE(double, DENSITY)
LOTUS_CREATE_VARIABLE(double, THICKNESS)

LOTUS_CREATE_VARIABLE(Constitutive_Law::Pointer, CONSTITUTIVE_LAW)