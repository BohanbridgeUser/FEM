#ifndef _STRATEGIES_H_
#define _STRATEGIES_H_
#include "../../define.h"
#include "../../Model/model.h"
#include "../schemes/solution_scheme.h"
#include "../../Container/flags.h"
#include "../solve_local_flags.h"
#include "../builder_and_solver/builder_and_solver.h"
#include "../convergencecriterias/convergencecriterias.h"
#include "../../Process/solver_process.h"
#include <Eigen/Sparse>
#include <Eigen/Dense>

template<typename TSparseSpace, 
         typename TDenseSpace, 
         typename TLinearSolver>
class Solve_Strategy : public Flags
{
    public:
        /// @name Type Define
        /// @{
            /* Utility Define */
            typedef Solver_Local_Flags
                                                                              LocalFlagType;
            typedef ModelPart
                                                                                 Model_Part;
            typedef Model_Part::DofsVectorType 
                                                                             DofsVectorType; 
            typedef Model_Part                                             
                                                                              ModelPartType;
            typedef typename TSparseSpace::SparseMatrix
                                                                               SpMatrixType; 
            typedef typename TSparseSpace::SparseVector
                                                                               SpVectorType;
            typedef SpMatrixType                                            
                                                                           GlobalMatrixType;
            typedef SpVectorType    
                                                                           GlobalVectorType;
            
            /* Solve_Strategy Define */ 
            typedef Solver_Process
                                                                                ProcessType;
            typedef typename Solver_Process::SharedPointer
                                                                         ProcessPointerType;
            typedef std::vector<ProcessPointerType>
                                                                   ProcessPointerVectorType;
            typedef Solve_Strategy<TSparseSpace,TDenseSpace,TLinearSolver> 
                                                                          Solve_StategyType;                                                      
            typedef Scheme<TSparseSpace,TDenseSpace>
                                                                                 SchemeType;
            typedef Builder_And_Solver<TSparseSpace,TDenseSpace,TLinearSolver>
                                                                       BuilderAndSolverType;
            LOTUS_POINTER_DEFINE(Solve_StategyType)       

        /// @}


        /// @name Life Circle
        /// @{
            /* Constructor */
            explicit Solve_Strategy()
            :Flags(),
             mEchoLevel(0)
            {

            }

            explicit Solve_Strategy(Model_Part& rModelPart)
            :Flags(),
             mrModelPart(rModelPart),
             mOptions(),
             mEchoLevel(0)
            {

            }

            Solve_Strategy(ModelPartType& rModelPart,Flags& rOptions)
            :Flags(),
             mrModelPart(rModelPart),
             mOptions(rOptions),
             mEchoLevel(0)
            {
            
            }
            /* Destructor */
            virtual ~Solve_Strategy()
            {

            }

        /// @}

        /// @name Operations
        /// @{
            /**
             * @brief The problem of interest is solved.
             * @details
             * {
             * This function calls sequentially: InitializeSolutionStep(), SolveSolutionStep() and FinalizeSolutionStep().
             * All those functions can otherwise be called separately.
             * }
             */
            virtual bool Solve()
            {
                this->InitializeSolutionStep();

                bool converged = this->SolveSolutionStep();

                // implementation of the adaptive time reduction
                if( this->IsNot(LocalFlagType::ADAPTIVE_SOLUTION) )
                converged = true;
                
                if(converged)
                this->FinalizeSolutionStep();

                return converged;
            }

            /**
             * @brief Performs all the required operations that should be done (for each step) before solving the solution step.
             * @details this function must be called only once per step.
             */
            virtual void InitializeSolutionStep() {}

            /**
             * @brief Performs all the required operations that should be done (for each step) after solving the solution step.
             * @details this function must be called only once per step.
             */
            virtual void FinalizeSolutionStep() {}

            /**
             * @brief Solves the current step. This function returns true if a solution has been found, false otherwise.
             */
            virtual bool SolveSolutionStep() {return true;}
            /**
             * @brief Initialization of member 
            */
            
            /**
             * @brief Solves the current iteration. This function returns true if a solution has been found, false otherwise.
             */
            virtual bool SolveIteration() {return true;}

            /**
             * @brief Function to perform expensive checks.
             * @details It is designed to be called ONCE to verify that the input is correct.
             */
            virtual int Check()
            {
                for (ModelPartType::ElementsContainerType::iterator it_elem = GetModelPart().ElementsBegin();
                    it_elem != GetModelPart().ElementsEnd(); it_elem++)
                {
                    it_elem->Check(GetModelPart().GetProcessInfo());
                }

                for (ModelPartType::ConditionsContainerType::iterator it_cond = GetModelPart().ConditionsBegin();
                    it_cond != GetModelPart().ConditionsEnd(); it_cond++)
                {
                    it_cond->Check(GetModelPart().GetProcessInfo());
                }
                return 0;
            }
        
        /// @}

        /// @name Access
        /// @{
            /**
             * @brief This sets the level of echo for the solution strategy
             * @param Level of echo for the solution strategy
             * @details
             * {
             * 0 -> Mute... no echo at all
             * 1 -> Printing time and basic informations
             * 2 -> Printing linear solver data
             * 3 -> Print of debug informations: Echo of stiffness matrix, Dx, b...
             * }
             */
            virtual void SetEchoLevel(const int Level)
            {
                mEchoLevel = Level;
            }

            /**
             * @brief This returns the level of echo for the solution strategy
             * @details
             * {
             * 0 -> Mute... no echo at all
             * 1 -> Printing time and basic informations
             * 2 -> Printing linear solver data
             * 3 -> Print of debug informations: Echo of stiffness matrix, Dx, b...
             * }
             * @return Level of echo for the solution strategy
             */
            virtual int GetEchoLevel()
            {
                return mEchoLevel;
            }

            /**
             * @brief Sets strategy options
             */
            void SetOptions(Flags& rOptions)
            {
                mOptions = rOptions;
            }

            /**
             * @brief Get strategy options
             @return mOptions: options member variable
            */
            Flags& GetOptions()
            {
                return mOptions;
            }

            /**
             * @brief This method gets the flag mMaxIterationNumber
             * @return mMaxIterationNumber: This is the maximum number of on linear iterations
             */
            virtual unsigned int GetMaxIterationNumber()
            {
                return 0;
            }

            /**
             * @brief Operations to get the pointer to the model
             * @return mrModelPart: The model part member variable
             */
            inline ModelPartType& GetModelPart()
            {
                return mrModelPart;
            };
        /// @}
    protected:
        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{
            /**
             * @brief Set Solver_Strategy Options
            */
            Flags mOptions;

            /**
             * @brief This sets the level of echo for the solving strategy
             * @param Level of echo for the solving strategy
             * @details
             * {
             * 0 -> Mute... no echo at all
             * 1 -> Printing time and basic information
             * 2 -> Printing linear solver data
             * 3 -> Print of debug information: Echo of stiffness matrix, Dx, b...
             * }
             */
            int mEchoLevel;
        /// @}


        /// @name Protected Operatiors
        /// @{


        /// @}


        /// @name Protected Operations
        /// @{
            /**
             * @brief Initialization of member variables and prior operations
             */
            virtual void Initialize(){};

            /**
             * @brief Operation to predict the solution ... if it is not called a trivial predictor is used in which the
             * values of the solution step of interest are assumed equal to the old values
             */
            virtual void Predict(){};

            /**
             * @brief Operation to update the solution ... if it is not called a trivial updater is used in which the
             * values of the solution step of interest are assumed equal to the old values
             */
            virtual void Update(){};

            /**
             * @brief Finalization of member variables and prior operations
             */
            virtual void Finalize(){};
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
            Model_Part& mrModelPart;
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