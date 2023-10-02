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
            typedef Solver_Local_Flags
                                                                                      LocalFlagType;
            typedef Builder_And_Solver<TSparseSpace,TDenseSpace,TLinearSolver> 
                                                                               BuilderAndSolverType;
            LOTUS_POINTER_DEFINE(BuilderAndSolverType)

            typedef Linear_Solver<TSparseSpace,TDenseSpace> 
                                                                                   LinearSolverType;
            typedef typename LinearSolverType::Pointer
                                                                            LinearSolverPointerType;
            typedef Scheme<TSparseSpace,TDenseSpace>
                                                                                         SchemeType;
            typedef typename SchemeType::Pointer
                                                                                  SchemePointerType;
            typedef ModelPart
                                                                                      ModelPartType;
            typedef ModelPart
                                                                                         Model_Part;
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
            typedef typename TSparseSpace::SparseMatrix
                                                                                       SpMatrixType;
            typedef typename TSparseSpace::SparseVector  
                                                                                       SpVectorType;
            typedef SpMatrixType  
                                                                                   GlobalMatrixType;
            typedef SpVectorType                                                     
                                                                                   GlobalVectorType;
            typedef typename std::shared_ptr<SpMatrixType>
                                                                            GlobalMatrixTypePointer;
            typedef typename std::shared_ptr<SpVectorType>
                                                                            GlobalVectorTypePointer;
            typedef typename TDenseSpace::Matrix
                                                                                    LocalMatrixType;
            typedef typename TDenseSpace::Vector
                                                                                    LocalVectorType; 
            typedef typename std::shared_ptr<LocalMatrixType>
                                                                             LocalMatrixPointerType;
            typedef typename std::shared_ptr<LocalVectorType>
                                                                             LocalVectorPointerType;                                                                        
        /// @}


        /// @name Life Circle
        /// @{
            explicit Builder_And_Solver()
            {

            }
            Builder_And_Solver(LinearSolverType& ThisLinearSolver)
            :mpLinearSolver(&ThisLinearSolver)
            {

            }
            virtual ~Builder_And_Solver()
            {

            }
        /// @}

        /// @name Operations 
        /// @{
            /* Utility Operations */
            Flags& GetOptions()
            {
                return mOptions;
            }
            unsigned int GetEquationGlobalSize() const
            {
                return mEquationSystemSize;
            }
            typename LinearSolverType::Pointer GetLinearSolver()const
            {
                return mpLinearSolver;
            }
            void SetLinearSolver(typename LinearSolverType::Pointer pLinearSolver)
            {
                mpLinearSolver = pLinearSolver;
            }
            void SetEchoLevel(int rEchoLevel)
            {
                mEchoLevel = rEchoLevel;
            }

            /* Solve Operations */
            /**
             * @brief Performs all the required operations that should be done (for each step) before solving the solution step.
             * @details this function must be called only once per step.
             */
            virtual void InitializeSolutionStep(SchemePointerType pScheme,
                                                Model_Part& rModelPart,
                                                GlobalMatrixTypePointer& pA,
                                                GlobalVectorTypePointer& pDx,
                                                GlobalVectorTypePointer& pb)
            {
            }
        
            /**
             * @brief Performs all the required operations that should be done (for each step) after solving the solution step.
             * @details this function must be called only once per step.
             */
            virtual void FinalizeSolutionStep(SchemePointerType pScheme,
                                                Model_Part& rModelPart,
                                                GlobalMatrixTypePointer& pA,
                                                GlobalVectorTypePointer& pDx,
                                                GlobalVectorTypePointer& pb)
            {
            }

            /**
             * @brief Calculates system reactions
             * @details A flag controls if reactions must be calculated
             * @details An internal variable to store the reactions vector is needed
             */
            virtual void CalculateReactions(SchemePointerType pScheme,
                                            Model_Part& rModelPart,
                                            GlobalMatrixTypePointer& rA,
                                            GlobalVectorTypePointer& rDx,
                                            GlobalVectorTypePointer& rb)
            {
            }

            virtual void BuildAndSolve(SchemeType& rScheme,
                                       ModelPartType& rModelPart,
                                       SpMatrixType& rmA,
                                       SpVectorType& rmDx,
                                       SpVectorType& rmb)
            {
                Build(rScheme,rModelPart,rmA,rmb);
                ApplyDirichletConditions(rScheme,rModelPart,rmA,rmDx,rmb);
                if (mEchoLevel == 3) 
                {
                    std::cout << "LHS Before Solve :\n" << rmA << std::endl;
                    std::cout << "DX  Before Solve :\n" << rmDx << std::endl;
                    std::cout << "RHS Before Solve :\n" << rmb << std::endl;
                }
                SystemSolveWithPhysics(rmA,rmDx,rmb,rModelPart);

                if (mEchoLevel == 3) 
                {
                    std::cout << "LHS After Solve :\n" << rmA << std::endl;
                    std::cout << "DX  After Solve :\n" << rmDx << std::endl;
                    std::cout << "RHS After Solve :\n" << rmb << std::endl;
                }
            }
            virtual void Build(SchemeType& rScheme,
                               ModelPartType& rModelPart,
                               SpMatrixType& rmA,
                               SpVectorType& rmb)
            {
            }
            virtual void ApplyDirichletConditions(SchemeType& rScheme,
                                                  ModelPartType& rModelPart,
                                                  SpMatrixType& rmA,
                                                  SpVectorType& rmDx,
                                                  SpVectorType& rmb)
            {
            }
            
            virtual void SystemSolveWithPhysics(SpMatrixType& rmA,
                                                SpVectorType& rDx,
                                                SpVectorType& rmb,
                                                ModelPartType& rModelPart)
            {
            }

            /**
             * @brief Builds the list of the DofSets involved in the problem by "asking" to each element and condition its Dofs.
             * @details The list of dofs is stores insde the SolutionBuilderAndSolver as it is closely connected to the way the matrix and RHS are built
             */
            virtual void SetUpDofSet(SchemePointerType pScheme,
                                    Model_Part& rModelPart)
            {
            }

            /**
             * @brief organises the dofset in order to speed up the building phase
             */
            virtual void SetUpSystem()
            {
            }

            virtual void Clear()
            {
                this->mDofSet.clear();

                if(this->mpReactionsVector != nullptr)
                    delete mpReactionsVector;
                
                mpLinearSolver->Clear();

                std::cout << "Builder And Solver Clear!\n";
            }
        /// @}

    protected:
        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{
            typename LinearSolverType::Pointer mpLinearSolver;

            /**
             * @brief The set contains the Dof of the system
            */
            DofsContainerType mDofSet;

            Flags mOptions;

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
            SpVectorType mpReactionsVector;

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
