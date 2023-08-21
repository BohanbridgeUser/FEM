#ifndef _LINEAR_SOLVING_STRATEGY_H_
#define _LINEAR_SOLVING_STRATEGY_H_

#include "solve_strategy.h"
#include "../builder_and_solver/block_builder_and_solver.h"

template<typename TSparseSpace, 
         typename TDenseSpace, 
         typename TLinearSolver>
class Linear_Solving_Strategy : public Solve_Strategy<TSparseSpace,TDenseSpace,TLinearSolver>
{
    public:
        /// @name Type Define
        /// @{
            /* Utility Define */
            typedef Solve_Strategy<TSparseSpace,TDenseSpace,TLinearSolver>
                                                                                             BaseType;
            typedef typename BaseType::DofsVectorType
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
            typedef typename SpMatrixType::Pointer
                                                                              GlobalMatrixTypePointer;
            typedef typename SpVectorType::Pointer
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

                mpA = new SpMatrixType;
                mpDx = new SpVectorType;
                mpb = new SpVectorType;
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

            virtual void FinalizedSolutionStep() override
            {
                if (this->mOptions.Is(LocalFlagType::COMPUTE_REACTIONS))
                    mpBuilderAndSolver->CalculateReactions(mpScheme,this->GetModelPart(),(*mpA),(*mpDx),(*mpb));

                mpScheme->FinalizeSolutionStep(this->GetModelPart);
                mpBuilderAndSolver->FinalizeSolutionStep(mpScheme,this->GetModelPart(),mpA,mpDx,mpb);
                
            }

            virtual void SolveSolutionStep()override
            {
                this->SolveIteration();
            }
            
            virtual void SolveIteration()override
            {
                if (!mpDx->size())
                {
                    std::cout << "mpDx is 0 dimension!\n";
                }

                if (this->mOptions.IsNot(LocalFlagType::CONSTANT_SYSTEM_MATRIX))
                {
                    TSparseSpace::Zero(*mpA);
                    TSparseSpace::Zero(*mpDx);
                    TSparseSpace::Zero(*mpb);
                    mpBuilderAndSolver->BuildAndSolve(mpScheme,this->GetModelPart,(*mpA),(*mpDx),(*mpb));
                }
                else
                {
                    TSparseSpace::Zero(*mpDx);
                    TSparseSpace::Zero(*mpb);
                    mpBuilderAndSolver->BuildAndSolve(mpScheme,this->GetModelPart,(*mpA),(*mpDx),(*mpb));
                }
                mpBuilderAndSolver->EchoInfo(this->GetModelPart,(*mpA),(*mpDx),(*mpb));
                this->Update();
                mpScheme->FinalizeNonLinearIteration(this->GetModelPart());
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

                this->Set(LocalFlagType::INITIALIZED,true);
            }

            void SetSystemDofs()
            {
                if (this->mEchoLevel >= 2)
                std::cout << " Reform Dofs " << " Flag = " << this->mOptions.Is(LocalFlagType::REFORM_DOFS) << std::endl;
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

            void Clear() override
            {
                /* Clear preconditioner */
                mpBuilderAndSolver->GetLinearSolver()->Clear();

                if (mpA != nullptr)
                    delete mpA;
                if (mpb != nullptr)
                    delete mpb;
                if (mpDx != nullptr)
                    delete mpDx;

                mpBuilderAndSolver->Clear();
                mpScheme->Clear();
            }

            void Update()override
            {
                mpScheme->Update(this->GetModelPart,mpBuilderAndSolver->GetDofSet(),(*mpDx));
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