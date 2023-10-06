#ifndef _LINEAR_SOLVING_STRATEGY_H_
#define _LINEAR_SOLVING_STRATEGY_H_

#include "solution_strategy.h"
#include "../builder_and_solver/block_builder_and_solver.h"

template<typename TSparseSpace, 
         typename TDenseSpace, 
         typename TLinearSolver>
class Linear_Solving_Strategy 
:public SolutionStrategy<TSparseSpace,TDenseSpace,TLinearSolver>
{
    public:
        /// @name Type Define
        /// @{
            /* Utility Define */
            typedef SolutionStrategy<TSparseSpace,TDenseSpace,TLinearSolver>
                                                                                             BaseType;
            typedef typename BaseType::DofsArrayType
                                                                                       DofsVectorType;
            typedef typename BaseType::LocalFlagType
                                                                                        LocalFlagType;
            typedef typename BaseType::ModelPartType
                                                                                        ModelPartType;
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
            /* Linear_Solving_Strategy */
            typedef Linear_Solving_Strategy<TSparseSpace,TDenseSpace,TLinearSolver> 
                                                                                            ClassType;                                                                           
            typedef typename BaseType::BuilderAndSolverType
                                                                                 BuilderAndSolverType;
            typedef typename BaseType::SchemeType
                                                                                           SchemeType;
            LOTUS_POINTER_DEFINE(ClassType)
        /// @}


        /// @name Life Circle
        /// @{
            explicit Linear_Solving_Strategy():BaseType()
            {

            }
            
            Linear_Solving_Strategy(ModelPartType& rModelPart,
                                    typename SchemeType::Pointer pScheme,
                                    typename BuilderAndSolverType::Pointer pBuilderAndSolver,
                                    Flags& rOptions)
            :BaseType(rModelPart,rOptions),
             mpScheme(pScheme),
             mpBuilderAndSolver(pBuilderAndSolver)
            {
                /* If the Linear Solver has to compute reactions, set the BuilderAndSolver compute reactions */
                mpBuilderAndSolver->GetOptions().Set(LocalFlagType::COMPUTE_REACTIONS,
                                                    this->mOptions.Is(LocalFlagType::COMPUTE_REACTIONS));
                
                /* If the Linear Solver has to reform Dofset, set the BuilderAndSolver reform Dofset */
                mpBuilderAndSolver->GetOptions().Set(LocalFlagType::REFORM_DOFS,
                                                    this->mOptions.Is(LocalFlagType::REFORM_DOFS));

                /* Set echolevel of BuilderAndSolver */
                mpBuilderAndSolver->SetEchoLevel(this->mEchoLevel);

                mpA  = std::make_shared<SpMatrixType>();
                mpDx = std::make_shared<SpVectorType>();
                mpb  = std::make_shared<SpVectorType>();
            }

            ~Linear_Solving_Strategy()
            {

            }
        /// @}

        /// @name Operators
        /// @{

        /// @}

        /// @name Operations
        /// @{
            virtual void InitializeSolutionStep() override
            {
                if (this->IsNot(LocalFlagType::INITIALIZED));
                    this->Initialize();
                
                if(this->mOptions.Is(LocalFlagType::REFORM_DOFS)){
                    this->Clear();
                    this->SetSystemDofs();
                }

                //initial operations ... things that are constant over the Solution Step
                mpBuilderAndSolver->InitializeSolutionStep(mpScheme, this->GetModelPart(), mpA, mpDx, mpb);

                //initial operations ... things that are constant over the Solution Step
                mpScheme->InitializeSolutionStep(this->GetModelPart());

                this->Predict();
            }

            bool SolveSolutionStep() override
            {
                return this->SolveIteration();
            }
            
            bool SolveIteration() override
            {
                // Warning info
                if(!(*mpDx).rows())
                    std::cout << ("DOFS") << "solution has zero size, no free DOFs" << std::endl;

                // Initialize Iteration
                mpScheme->InitializeNonLinearIteration(this->GetModelPart());

                //function to perform the building and the solving phase.
                if(this->mOptions.IsNot(LocalFlagType::CONSTANT_SYSTEM_MATRIX)){

                (*mpA ).setZero();
                (*mpDx).setZero();
                (*mpb).setZero();

                mpBuilderAndSolver->BuildAndSolve(mpScheme, this->GetModelPart(), (*mpA), (*mpDx), (*mpb));
                }
                else{

                (*mpDx).setZero();
                (*mpb).setZero();

                mpBuilderAndSolver->BuildRHSAndSolve(mpScheme, this->GetModelPart(), (*mpA), (*mpDx), (*mpb));
                }

                // EchoInfo
                mpBuilderAndSolver->EchoInfo(this->GetModelPart(), (*mpA), (*mpDx), (*mpb));

                // Updating the results
                this->Update();

                // Finalize Iteration
                mpScheme->FinalizeNonLinearIteration(this->GetModelPart());
                return true;
            }
        
            void FinalizeSolutionStep() override
            {
                //calculate reactions if required
                if(this->mOptions.Is(LocalFlagType::COMPUTE_REACTIONS))
                    mpBuilderAndSolver->CalculateReactions(mpScheme, this->GetModelPart(), (*mpA), (*mpDx), (*mpb));

                //finalize scheme anb builder and solver
                mpScheme->FinalizeSolutionStep(this->GetModelPart());
                mpBuilderAndSolver->FinalizeSolutionStep(mpScheme, this->GetModelPart(), mpA, mpDx, mpb);
            }

            /**
             * @brief Clears the internal storage
             */
            void Clear() override
            {
                // if the preconditioner is saved between solves, it should be cleared here.
                mpBuilderAndSolver->GetLinearSystemSolver()->Clear();

                //deallocate the systemvectors
                if(mpA != nullptr)
                    mpA = nullptr;
                if(mpDx != nullptr)
                    mpDx = nullptr;
                if(mpb != nullptr)
                    mpb = nullptr;

                mpBuilderAndSolver->Clear();
                mpScheme->Clear();
            }

            /**
             * @brief Function to perform expensive checks.
             * @details It is designed to be called ONCE to verify that the input is correct.
             */
            int Check() override
            {
                //check the model part
                BaseType::Check();
                //check the scheme
                mpScheme->Check(this->GetModelPart());
                //check the builder and solver
                mpBuilderAndSolver->Check(this->GetModelPart());
                return 0;

            }

        /// @}

        /// @name Access
        /// @{
            void SetEchoLevel(const int level)override
            {
                BaseType::SetEchoLevel(level);
                mpBuilderAndSolver->SetEchoLevel(level);
            }
            void SetScheme(typename SchemeType::SharedPointer rpScheme)
            {
                mpScheme = rpScheme;
            }
            typename SchemeType::SharedPointer GetScheme()
            {
                return mpScheme;
            }
            void SetBuilderAndSolver(typename BuilderAndSolverType::Pointer pBuilderAndSolver)
            {
                mpBuilderAndSolver = pBuilderAndSolver;
            }
            typename BuilderAndSolverType::Pointer GetBuilderAndSolver()
            {
                return mpBuilderAndSolver;
            }
            SpMatrixType& GetSystemMatrix()
            {
                return *mpA;
            }
        /// @}

        /// @name Inquiry
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
            void Initialize() override
            {
                if( mpScheme->IsNot(LocalFlagType::INITIALIZED))
                    mpScheme->Initialize(this->GetModelPart());

                this->SetSystemDofs();
                
                unsigned int sizeofsystem = mpBuilderAndSolver->GetEquationSystemSize();
                std::cout << "SystemMatrix Size: " << sizeofsystem << std:: endl;
                (*mpA).resize(sizeofsystem,sizeofsystem);
                (*mpb).resize(sizeofsystem);
                (*mpDx).resize(sizeofsystem);
                this->Set(LocalFlagType::INITIALIZED,true);
            }

            void SetSystemDofs()
            {
                if (this->mEchoLevel >= 2)
                    std::cout << " Reform Dofs " 
                              << " Flag = " 
                              << this->mOptions.Is(LocalFlagType::REFORM_DOFS) 
                              << std::endl;
                //setting up the list of the DOFs to be solved
                // double begin_time = OpenMPUtils::GetCurrentTime();
                mpBuilderAndSolver->SetUpDofSet(mpScheme, this->GetModelPart());
                // double end_time = OpenMPUtils::GetCurrentTime();
                // begin_time = OpenMPUtils::GetCurrentTime();
                mpBuilderAndSolver->SetUpSystem();
                // end_time = OpenMPUtils::GetCurrentTime();
            }

            void Predict() override
            {
                mpScheme->Predict(this->GetModelPart(), mpBuilderAndSolver->GetDofSet(), (*mpDx));
            }

            void Update()override
            {
                mpScheme->Update(this->GetModelPart(),mpBuilderAndSolver->GetDofSet(),(*mpDx));
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
            typename SchemeType::Pointer mpScheme;
            typename BuilderAndSolverType::Pointer mpBuilderAndSolver;

            GlobalMatrixTypePointer mpA;
            GlobalVectorTypePointer mpb;
            GlobalVectorTypePointer mpDx;
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