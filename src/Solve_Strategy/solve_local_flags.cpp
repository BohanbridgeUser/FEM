#include "../../include/Solve_Strategy/solve_local_flags.h"

/**
 * FLAGSs for the solution control
 */
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, INITIALIZED,               0 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, CONVERGED,                 1 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, DOFS_INITIALIZED,          2 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, ELEMENTS_INITIALIZED,      3 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, CONDITIONS_INITIALIZED,    4 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, ADAPTIVE_SOLUTION,         5 )

/**
 * FLAGSs for the solution options
 */
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, MOVE_MESH,                 0 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, UPDATE_VARIABLES,          1 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, REFORM_DOFS,               2 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, INCREMENTAL_SOLUTION,      3 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, COMPUTE_REACTIONS,         4 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, CONSTANT_SYSTEM_MATRIX,    5 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, RAYLEIGH_DAMPING,          6 )
LOTUS_CREATE_LOCAL_FLAGS( Solver_Local_Flags, IMPLEX,                    7 )


/**
 * FLAGSs for the convergence criterion control
 */
LOTUS_CREATE_LOCAL_FLAGS( Criterias_Local_Flags, INITIALIZED,               0 )
LOTUS_CREATE_LOCAL_FLAGS( Criterias_Local_Flags, INCREMENTAL,               1 )
LOTUS_CREATE_LOCAL_FLAGS( Criterias_Local_Flags, CONVERGED,                 2 )
LOTUS_CREATE_LOCAL_FLAGS( Criterias_Local_Flags, AND,                       3 )
LOTUS_CREATE_LOCAL_FLAGS( Criterias_Local_Flags, OR,                        4 )
LOTUS_CREATE_LOCAL_FLAGS( Criterias_Local_Flags, UPDATE_RHS,                5 )
LOTUS_CREATE_LOCAL_FLAGS( Criterias_Local_Flags, SUPPLIED_DOF,              6 )

/**
 * FLAGSs for the time integration options
 */
LOTUS_CREATE_LOCAL_FLAGS( Time_Integration_Local_Flags, PREDICT_PRIMARY_VARIABLE,   0 )
