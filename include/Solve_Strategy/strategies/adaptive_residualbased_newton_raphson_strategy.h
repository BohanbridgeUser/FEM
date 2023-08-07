#ifndef _ADAPTIVE_RESIDUALBASED_NEWTON_RAPHSON_STRATEGY_H_
#define _ADAPTIVE_RESIDUALBASED_NEWTON_RAPHSON_STRATEGY_H_
#include "implicit_solving_strategy.h"

template<typename TSparseSpace, typename TDenseSpace, typename TLinearSolver>
class AdaptiveResidualBasedNewtonRaphsonStrategy : public Implicit_Solving_Strategy<TSparseSpace, TDensSpace, TLinearSolver>
{
    public:
        /// @name Type Define
        /// @{
            typedef AdaptiveResidualBasedNewtonRaphsonStrategy<TSparseSpace,TDenseSpace,TLinearSolver> 
                                                                                                        ClassType;
            LOTUS_POINTER_DEFINE(ClassType)
            
            typedef Solve_Stategy<TSparseSpace,TDenseSpace, TLinearSolver> 
                                                                                                SolveStrategyType;
            
            typedef Implicit_Solving_Strategy<TSparseSpace, TDenseSpace, TLinearSolver> 
                                                                                      ImplicitSolvingStrategyType;

            typedef Scheme<TSparseSpace, TDenseSpace> 
                                                                                                       SchemeType;

            typedef Builder_And_Solver<TSparseSpace,TDenseSpace,TLinearSolver> 
                                                                                             BuilderAndSolverType;

            typedef ConvergenceCriterias<TSparseSpace,TDenseSpace>
                                                                                         ConvergenceCriteriasType;
            typedef TSparseSpace::SparseMatrix<double>
                                                                                                     SpMatrixType;
            typedef TSparseSpace::SparseVector<double>
                                                                                                     SpVectorType;
            typedef TDenseSpace
                                                                                                       MatrixType;
            typedef TLinearSolver
                                                                                                 LinearSolverType;
            typedef Model_Part
                                                                                                    ModelPartType;
            

        /// @}

        /// @name Life Circle
        /// @{
            /* Constructor */
            explicit AdaptiveResidualBasedNewtonRaphsonStrategy():ImplicitSolvingStrategyType()
            {

            }

            AdaptiveResidualBasedNewtonRaphsonStrategy(
                ModelPartType& rModelPart,
                typename SchemeType::Pointer pScheme,
                typename LinearSolver::Pointer pNewLinearSolver,
                typename ConvergenceCriteriaType::Pointer pNewConvergenceCriteria,
                int MaxIterations = 30,
                int MinIterations = 4,
                bool CalculateReactions = false,
                bool ReformDofSetAtEachStep = false,
                bool MoveMeshFlag = false,
                double ReductionFactor = 0.5,
                double IncreaseFactor = 1.3,
                int NumberOfCycles = 5
                ) : SolveStrategyType(rModelPart)
            {
                /* Set Iteration Number */
                SetMaxIterationNumber(MaxIterations);
                mMinIterationNumber = MinIterations;
                mReductionFactor = ReductionFactor;
                mIncreaseFactor = IncreaseFactor;
                mNumberOfCycles = NumberOfCycles;
                /* Set Calculation Flag */
                mCalculateReactionsFlag = CalculateReactions;
                mReformDofSetAtEachStep = ReformDofSetAtEachStep;
                mSolutionStepIsInitialized = false;
                mInitializeWasPerformed = false;
                /* Set Convergence Criteria */
                mpConvergenceCriterias = pNewConvergenceCriteria;
                /* Set Scheme */
                mpScheme = pScheme;
                /* Set the linear solver */
                mpLinearSolver = pNewLinearSolver;
                /**
                 * @brief Set the Builder And Solver 
                 * @details The adaptive residualbased newton raphson strategy using Residual Based Elimination Builder And Solver
                 * @param  
                 * - ResidualBasedEliminationBuilderAndSolver
                 * 
                 */
                mpBuilderAndSolver = typename TBuilderAndSolverType::Pointer
                                    (
                                        new ResidualBasedEliminationBuilderAndSolver<TSparseSpace,TDenseSpace,TLinearSolver>(mpLinearSolver)
                                    );

                /*tells to the builder and solver if the reactions have to be Calculated or not */
                GetBuilderAndSolver()->SetCalculateReactionsFlag(mCalculateReactionsFlag);

                //tells to the Builder And Solver if the system matrix and vectors need to
                //be reshaped at each step or not
                GetBuilderAndSolver()->SetReshapeMatrixFlag(mReformDofSetAtEachStep);

                //set EchoLevel to the default value (only time is displayed)
                SetEchoLevel(1);

                //by default the matrices are rebuilt at each iteration
                this->SetRebuildLevel(2);
            }


            /* Destructor */
            virtual AdaptiveResidualBasedNewtonRaphsonStrategy()
            {

            }
        /// @}

        /// @name Public Operations
        /// @{
            void SetMaxIterationNumber(int MaxIterations)
            {
                mMaxIterationNumber = MaxIterations;
            }
            typename BuilderAndSolverType::Pointer GetBuildAndSolver()
            {
                return mpBuilderAndSolver;
            }
        /// @}

    protected:
        /// @name Protected static Member Variables 
        /// @{

        ///@}

        /// @name Protected Member Variables 
        /// @{
            typename SchemeType::SharedPointer mpScheme;
        
            typename TLinearSolver::SharedPointer mpLinearSolver;

            typename BuilderAndSolverType::SharedPointer mpBuilderAndSolver;

            typename ConvergenceCriteriasType::SharedPointer mpConvergenceCriterias;

            /**
             *  @brief Global Equation
             *  @details ma * mDx = mb;
             * 
            */
            typename SpVectorType mDx;

            typename SpVectorType mb;

            typename SpMatrixType ma;

            /**
             * @brief Flag telling if it is needed to reform the DofSet at each solution step 
             * or if it is possible to form it just once
             * - true  => reforme at each time step
             * - false => form just one (more efficient)
             * Default = false
            */
            bool mReformDofSetAtEachStep;

            /**
             * @brief Flag telling if it is needed or not to compute the reactions
             * @details Default = true
            */
            bool mCalculateReactionsFlag;

            /**
             * @brief Flag telling if it's SolutionStep is initialized
            */
            bool mSolutionStepIsInitialized;

            /**
             * @brief Set the max iteration number of step
             * @details Default = 30
            */
            int mMaxIterationNumber;
            int mMinIterationNumber;
            bool mInitializeWasPerformed;

            double mReductionFactor;
            double mIncreaseFactor;

            int mNumberOfCycles;
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
        /// @name Private static Member Variables 
        /// @{

        ///@}

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
