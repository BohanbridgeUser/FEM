#ifndef _block_builder_and_solver_h_
#define _block_builder_and_solver_h_
#include "builder_and_solver.h"
template<class TSparseSpace,
         class TDenseSpace, //= DenseSpace<double>,
         class TLinearSolver //= LinearSolver<TSparseSpace,TDenseSpace>
         >
class Block_Builder_And_Solver : public Builder_And_Solver<TSparseSpace,TDenseSpace,TLinearSolver>
{
    public:
        /// @name Type Define
        /// @{
            typedef Block_Builder_And_Solver<TSparseSpace,TDenseSpace,TLinearSolver>
                                                                                             ClassType;
            LOTUS_POINTER_DEFINE(ClassType)                                                                                 
            typedef Builder_And_Solver<TSparseSpace,TDenseSpace,TLinearSolver>
                                                                                              BaseType;
            

        /// @}


        /// @name Life Circle
        /// @{


        /// @}


    protected:
        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{


        /// @}


        /// @name Protected Operatiors
        /// @{


        /// @}


        /// @name Protected Operations
        /// @{


        /// @}


        /// @name Protected Access
        /// @{


        /// @}


        /// @name Protected Inquiry
        /// @{


        /// @}


    private:
        /// @name Private Static Member Variables
        /// @{


        /// @}


        /// @name Private Member Variables
        /// @{
            Flags mOptions;

        /// @}


        /// @name Private Operatiors
        /// @{


        /// @}


        /// @name Private Operations
        /// @{


        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};
#endif