#ifndef _RESIDUALBASED_ELIMINATION_BUILDEF_AND_SOLVER_H_
#define _RESIDUALBASED_ELIMINATION_BUILDEF_AND_SOLVER_H_
#include "builder_and_solver.h"
template<typename TSparseSpace, typename TDenseSpace, typename TLinearSolver>
class ResidualbasedEliminationBuildefAndSolver : public Builder_And_Solver<TSparseSpace,TDenseSpace,TLinearSolver>
{
    public:
        /// @name Type Define
        /// @{
            typedef ResidualbasedEliminationBuildefAndSolver<TSparseSpace,TDenseSpace,TLinearSolver> 
                                                                                                    ClassType;
            LOTUS_POINTER_DEFINE(ClassType)
            
            typedef Builder_And_Solver<TSparseSpace,TDenseSpace,TLinearSolver>
                                                                                                     BaseType;
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
            typedef typedef ConvergenceCriterias<TSparseSpace,TDenseSpace> 
                                                                                      ConvergenceCriteriaType;
        /// @}


        /// @name Life Circle
        /// @{
             /* Constructor */
            explicit ResidualbasedEliminationBuildefAndSolver()
            {

            }
            ResidualbasedEliminationBuildefAndSolver(typename LinearSolverType& ThisLinearSolver)
            :BaseType(ThisLinearSolver)
            {

            }

            /* Destructor */
            virtual ~ResidualbasedEliminationBuildefAndSolver()
            {

            }
        /// @}

        /// @name Operators
        /// @{

        /// @}

        /// @name Operations
        /// @{
            void Build()
            {

            }
            /**
             * @brief Function to perform the building of the LHS
             * @details Depending on the implementation chosen the size of the matrix could be equal to the total number of Dofs or to the number of unrestrained dofs
             * @param pScheme The integration scheme considered
             * @param rModelPart The model part of the problem to solve
             * @param rA The LHS matrix
             */
            void BuildLHS()
            {

            }
            /**
             * @brief Build a rectangular matrix of size n*N where "n" is the number of unrestrained degrees of freedom
             * and "N" is the total number of degrees of freedom involved.
             * @details This matrix is obtained by building the total matrix without the lines corresponding to the fixed
             * degrees of freedom (but keeping the columns!!)
             * @param pScheme The integration scheme considered
             * @param rModelPart The model part of the problem to solve
             * @param A The LHS matrix
             */
            void BuildLHS_CompleteOnFreeRows()
            {

            }

            /**
             * @brief This funcion is a call to the linear Solver
             * @param 
             * 
            */
            void LinearSolver()
            {

            }

            /**
             * @brief This funcion is a call to the linear Solver with physical particularties of the problem
             * @param 
             * 
            */
            void LinearSolveWithPhysics()
            {

            }

            /**
             * @brief Function to perform the building and solving phase at the same time.
             * @details It is ideally the fastest and safer function to use when it is possible to solve
             * just after building
             * @param pScheme The integration scheme considered
             * @param rModelPart The model part of the problem to solve
             * @param rA The LHS matrix
             * @param rDx The Unknowns vector
             * @param rb The RHS vector
             */
            void BuildAndSolve()
            {

            }

            /**
             * @brief Corresponds to the previews, but the System's matrix is considered already built and only the RHS is built again
             * @param pScheme The integration scheme considered
             * @param rModelPart The model part of the problem to solve
             * @param rA The LHS matrix
             * @param rDx The Unknowns vector
             * @param rb The RHS vector
             */
            void BuildRHSAndSolve()
            {

            }

            /**
             * @brief Function to perform the build of the RHS.
             * @details The vector could be sized as the total number of dofs or as the number of unrestrained ones
             * @param pScheme The integration scheme considered
             * @param rModelPart The model part of the problem to solve
             */
            void BuildRHS()
            {

            }

            /**
             * @brief Builds the list of the DofSets involved in the problem by "asking" to each element
             * and condition its Dofs.
             * @details The list of dofs is stores inside the BuilderAndSolver as it is closely connected to the
             * way the matrix and RHS are built
             * @param pScheme The integration scheme considered
             * @param rModelPart The model part of the problem to solve
             */
            void SetUpDofSet()
            {

            }

            /**
             * @brief Organises the dofset in order to speed up the building phase
             * @param rModelPart The model part of the problem to solve
             */
            void SetUpSystem()
            {
                // Set equation id for degrees of freedom
                // the free degrees of freedom are positioned at the beginning of the system,
                // while the fixed one are at the end (in opposite order).
                //
                // that means that if the EquationId is greater than "mEquationSystemSize"
                // the pointed degree of freedom is restrained
                //
            }

            /**
             * @brief This method resize and initializes the system(global) of euqations
             * @param pA The pointer to the LHS matrix
             * @param pDx The pointer to the vector of Unknowns
             * @param pb The pointer to the RHS vector
             * @param rModelPart The model part to be computed
             */
            void ResizeAndInitializeVectors()
            {

            }

            /**
             * @brief This method computes the reactions
             * @param pScheme The integration scheme considered
             * @param rModelPart The model part considered
             * @param rA The LHS of the system
             * @param rDx The vector of Unknowns
             * @param rb The RHS vector
             */
            void CalculateReactions()
            {

            }

            /**
             * @brief Applies the dirichlet conditions. This operation may be very heavy or completely
             * unexpensive depending on the implementation chosen and on how the System Matrix is built.
             * @details For explanation of how it works for a particular implementation the user
             * should refer to the particular Builder And Solver chosen
             * @param pScheme The integration scheme considered
             * @param rModelPart The model part of the problem to solve
             * @param rA The LHS matrix
             * @param rDx The Unknowns vector
             * @param rb The RHS vector
             */
            void ApplyDirichletConditions()
            {
                
            }

            /**
             * @brief This function is intended to be called at the end of the solution step to clean up memory storage not needed
             */
            void Clear()
            {

            }

            /**
             * @brief Returns the name of the class as used in the settings (snake_case format)
             * @return The name of the class
             */
            static std::string Name()
            {
                return "elimination_builder_and_solver";
            }
        /// @}

    protected:
        /// @name Protected Static Member Variables
        /// @{
           
        /// @}


        /// @name Protected Member Variables
        /// @{
            /// The manually set scale factor
            double mScaleFactor = 1.00;         

            /// We identify the scaling considered for the dirichlet dofs
            double mScalingDiagonal = 1.00;
        /// @}


        /// @name Protected Operatiors
        /// @{


        /// @}


        /// @name Protected Operations
        /// @{
            /**
            * @brief This method assembles the system
            * @param rA The LHS of the system
            * @param rb The RHS of the system
            * @param rLHSContribution The LHS local contribution
            * @param rRHSContribution The RHS local contribution
            * @param rEquationId The equation id
            * @param rLockArray The lock of the dof
            * @note The main difference respect the block builder and solver is the fact that the fixed DoFs are not considered on the assembling
            */
            void Assemble()
            {

            }
            /**
             * @brief This method constructs the relationship between the DoF
             * @param pScheme The integration scheme
             * @param rA The LHS of the system
             * @param rModelPart The model part which defines the problem
             */
            virtual void ConstructMatrixStructure()
            {

            }
            /**
             * @brief This method assembles the LHS of the system
             * @param rA The LHS to assemble
             * @param rLHSContribution The local LHS contribution
             * @param rEquationId The equation id
             */
            void AssembleLHS()
            {

            }
            /**
             * @brief This function is equivalent to the AssembleRowContribution of the block builder and solver
             * @note The main difference respect the block builder and solver is the fact that the fixed DoFs are skipped
             */
            inline void AssembleRowContributionFreeDofs()
            {

            }
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
            /**
             * @brief This method assembles the RHS of the system
             * @param rb The RHS to assemble
             * @param rRHSContribution The local RHS contribution
             * @param rEquationId The equation id
             */
            void AssembleRHS()
            {

            }
            /**
             * @brief This method assembles the LHS of the system (on free rows)
             * @param rA The LHS to assemble
             * @param rLHSContribution The local LHS contribution
             * @param rEquationId The equation id
             */
            void AssembleLHSCompleteOnFreeRows()
            {
                
            }
        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};

#endif