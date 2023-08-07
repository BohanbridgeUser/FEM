#ifndef _IMPLICIT_SOLVING_STRATEGY_H_
#define _IMPLICIT_SOLVING_STRATEGY_H_
#include "solve_strategy.h"

template<typename TSparseSpace, typename TDenseSpace, typename TLinearSolver>
class  Implicit_Solving_Strategy: public Solve_Stategy<TSparseSpace, TDenseSpace, TLinearSolver>
{
    public:
        /// @name Define
        /// @{
            typedef Implicit_Solving_Strategy<TSparseSpace,TDenseSpace,TLinearSolver> ClassType;
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
        /// @}

        /// @name Life Circle
        /// @{
            explicit Implicit_Solving_Strategy():BaseType()
            {

            }
            explicit Implicit_Solving_Strategy(Model_Part& rModelPart)
                                                :BaseType(rModelPart)
            {

            }
            virtual ~Implicit_Solving_Strategy()
            {

            }
        /// @}
        
        /// @name Operations
        /// @{
            static std::string Name()
            {
                return "Implicit_Solving_Strategy\n";
            }

            /**
             * @brief This sets the build level
             * @param Level The build level
             * @details
             * {
             * 0 -> Build StiffnessMatrix just once
             * 1 -> Build StiffnessMatrix at the beginning of each solution step
             * 2 -> build StiffnessMatrix at each iteration
             * }
             */
            void SetRebuildLevel(int Level) override
            {
                mRebuildLevel = Level;
                mStiffnessMatrixIsBuilt = false;
            }

            /**
             * @brief This funcion returns the build level
             * @details
             * {
             * 0 -> Build StiffnessMatrix just once
             * 1 -> Build StiffnessMatrix at the beginning of each solution step
             * 2 -> build StiffnessMatrix at each iteration
             * }
            */
            int GetRebuildLevel() const 
            {
                    return mRebuildLevel;
            }

            /**
             * @brief This method sets the flag mStiffnessMatrixIsBuilt
             * @param StiffnessMatrixIsBuilt The flag that tells if the stiffness matrix is built
             */
            void SetStiffnessMatrixIsBuilt(const bool StiffnessMatrixIsBuilt)
            {
                mStiffnessMatrixIsBuilt = StiffnessMatrixIsBuilt;
            }

            /**
             * @brief This method gets the flag mStiffnessMatrixIsBuilt
             * @return mStiffnessMatrixIsBuilt: The flag that tells if the stiffness matrix is built
             */
            bool GetStiffnessMatrixIsBuilt() const
            {
                return mStiffnessMatrixIsBuilt;
            }

        /// @}
    protected:
        /// @name Protected static member variables
        /// @{

        /// @}
        
        /// @name Protected member variables
        /// @{
            /**
             * @param mRebuildLevel
             * @details
             * {
             * 0 -> Build StiffnessMatrix just once
             * 1 -> Build StiffnessMatrix at the beginning of each solution step
             * 2 -> build StiffnessMatrix at each iteration
             * }
             */
            int mRebuildLevel;            /// The current rebuild level


            bool mStiffnessMatrixIsBuilt; /// A flag indicating if the stiffness matrix has been built
        /// @}
    private:
        ///@name Static Member Variables
        ///@{


        ///@}
        ///@name Member Variables
        ///@{


        ///@}
        ///@name Private Operators
        ///@{


        ///@}
        ///@name Private Operations
        ///@{


        ///@}
        ///@name Private  Access
        ///@{


        ///@}
        ///@name Private Inquiry
        ///@{


        ///@}
        ///@name Un accessible methods
        ///@{

        ///@}
};

#endif