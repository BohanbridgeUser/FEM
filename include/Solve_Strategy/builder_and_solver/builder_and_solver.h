#ifndef _BUILDER_AND_SOLVER_H_
#define _BUILDER_AND_SOLVER_H_
#include "../../define.h"
#include "../../Model/model.h"
#include "../../Linear_Solver/linear_solver.h"
#include "../schemes/scheme.h"
#include "../convergencecriterias/convergencecriterias.h"
template<typename TSparseSpace, typename TDenseSpace, typename TLinearSolver>
class Builder_And_Solver
{
    public:
        /// @name Type Define
        /// @{
            typedef Builder_And_Solver<TSparseSpace,TDenseSpace,TLinearSolver> 
                                                                               BuilderAndSolverType;
            LOTUS_POINTER_DEFINE(BuilderAndSolverType)

            typedef LinearSolver<TSparseSpace,TDenseSpace> 
                                                                                   LinearSolverType;
            typedef Scheme<TSparseSpace,TDenseSpace>
                                                                                         SchemeType;
            typedef Model_Part
                                                                                      ModelPartType;
            typedef Model_Part::DofType                                                 
                                                                                            DofType;
            typedef Model_Part::DofsVectorType
                                                                                  DofsContainerType;
            typedef Model_Part::NodeType
                                                                                           NodeType;
            typedef Model_Part::NodesContainerType
                                                                                 NodesContainerType;
            typedef Model_Part::ElementsContainerType   
                                                                              ElementsContainerType;
            typedef Model_Part::ConditionsContainerType
                                                                            ConditionsContainerType;
            typedef TSparseSpace::SparseMatrix<double>
                                                                                   SparseVectorType;
        /// @}


        /// @name Life Circle
        /// @{
            explicit Builder_And_Solver()
            {

            }
            Builder_And_Solver(typename LinearSolverType& ThisLinearSolver)
            :mpLinearSolver(&ThisLinearSolver)
            {

            }
            virtual ~Builder_And_Solver()
            {

            }
        /// @}


    protected:
        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{
            typename LinearSolverType::SharedPoint mpLinearSolver;

            /**
             * @brief The set contains the Dof of the system
            */
            DofsContainerType mDofSet;

            bool mReshapeMatrixFlag = false;

            /**
             * @brief This flag telling if it is initialized or not
            */
            bool mDofSetIsInitialized = false; 

            /**
             * @brief This flag telling if it is needed or not to compute the reactions
            */
            bool mCalculateReactionsFlag = false; 

            /**
             * @brief Total number of degrees of freedom of this problem to be solve
            */
            unsigned int mEquationSystemSize;
            
            /**
             * @brief Echo level tells how many informations should be output
             * @details 
             * - 0: Mute... no echo at all
             * - 1: Printing time and basic information
             * - 2: Printing linear solver data
             * - 3: Print of debug information: Echo of stiffness matrix, Dx, b...
             * - 4: Print of stiffness matrix, b to Matrix Market
            */
            int mEchoLevel = 0;
            
            /**
             * @brief Reactions vector
            */
            SparseVectorType mpReactionsVector;

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
