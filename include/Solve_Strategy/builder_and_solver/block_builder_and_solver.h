#ifndef _block_builder_and_solver_h_
#define _block_builder_and_solver_h_
#include "builder_and_solver.h"
#include "../../key_hash.h"

#include <unordered_set>
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
            typedef typename BaseType::Pointer                                       
                                                                  BasePointerType;
            typedef typename BaseType::LocalFlagType                                   
                                                                    LocalFlagType;
            typedef typename BaseType::DofsArrayType                                   
                                                                    DofsArrayType;
            typedef typename BaseType::GlobalMatrixType                             
                                                                 GlobalMatrixType;
            typedef typename BaseType::GlobalVectorType                             
                                                                 GlobalVectorType;
            typedef typename BaseType::GlobalMatrixPointerType               
                                                          GlobalMatrixPointerType;
            typedef typename BaseType::GlobalVectorPointerType  
                                                          GlobalVectorPointerType;
            typedef typename BaseType::LocalVectorType          
                                                                  LocalVectorType;
            typedef typename BaseType::LocalMatrixType          
                                                                  LocalMatrixType;
            typedef typename Model_Part::NodesContainerType         
                                                               NodesContainerType;
            typedef typename Model_Part::ElementsContainerType  
                                                            ElementsContainerType;
            typedef typename Model_Part::ConditionsContainerType
                                                          ConditionsContainerType;
            typedef typename BaseType::SchemePointerType           
                                                                SchemePointerType;
            typedef typename BaseType::LinearSolverPointerType  
                                                          LinearSolverPointerType;
            struct dof_iterator_hash
            {
                size_t operator()(const Node::DofType::Pointer& it) const
                {
                    std::size_t seed = 0;
                    HashCombine(seed, it->Id());
                    HashCombine(seed, (it->GetVariable()).Key());
                    return seed;
                }
            };
            struct dof_iterator_equal
            {
                size_t operator()(const Node::DofType::Pointer& it1, const Node::DofType::Pointer& it2) const
                {
                    return (((it1->Id() == it2->Id() && (it1->GetVariable()).Key())==(it2->GetVariable()).Key()));
                }
            };
        /// @}


        /// @name Life Circle
        /// @{
            /* Default Constructor */
            Block_Builder_And_Solver(LinearSolverPointerType pLinearSystemSolver)
                : BaseType(pLinearSystemSolver)
            {

            }
            /* Destructor. */ 
            ~Block_Builder_And_Solver() override
            {

            }
        /// @}

        /// @name Operators
        /// @{

        /// @}

        /// @name Opeartions
        /// @{
            /**
             * @brief Function to perform the building of the LHS,
             * @details Depending on the implementation choosen the size of the matrix could be
             * @details equal to the total number of Dofs or to the number of non constrained dofs
             */
            void BuildLHS(SchemePointerType pScheme,
                            Model_Part& rModelPart,
                            GlobalMatrixType& rA) override
            {
                GlobalVectorType tmp(rA.size1(), 0.0);
                this->Build(pScheme, rModelPart, rA, tmp);
            }

            /**
             * @brief Function to perform the build of the RHS.
             * @details The vector could be sized as the total number of dofs or as the number of non constrained ones
             */
            void BuildRHS(SchemePointerType pScheme,
                            Model_Part& rModelPart,
                            GlobalVectorType& rb) override
            {
                BuildRHSNoDirichlet(pScheme,rModelPart,rb);
                const int ndofs = static_cast<int>(this->mDofSet.size());
                //NOTE: dofs are assumed to be numbered consecutively in the BlockBuilderAndSolver
                for (int k = 0; k<ndofs; k++)
                {
                    typename DofsArrayType::iterator dof_iterator = this->mDofSet.begin() + k;
                    const std::size_t i = dof_iterator->EquationId();
                    if (dof_iterator->IsFixed())
                        rb[i] = 0.0f;
                }
            }

            /**
             * @brief Function to perform the building of the LHS and RHS
             * @details Equivalent (but generally faster) then performing BuildLHS and BuildRHS
             */
            void Build(SchemePointerType pScheme,
                        Model_Part& rModelPart,
                        GlobalMatrixType& rA,
                        GlobalVectorType& rb) override
            {

                if (!pScheme){
                    std::cerr  << "No scheme provided!" << std::endl;
                    exit(0);
                }
                //getting the elements from the model
                const int nelements = static_cast<int>(rModelPart.Elements().size());

                //getting the array of the conditions
                const int nconditions = static_cast<int>(rModelPart.Conditions().size());

                Process_Info& rCurrentProcessInfo = rModelPart.GetProcessInfo();
                Model_Part::ElementsContainerType::iterator el_begin = rModelPart.ElementsBegin();
                Model_Part::ConditionsContainerType::iterator cond_begin = rModelPart.ConditionsBegin();

                //contributions to the system
                LocalMatrixType LHS_Contribution = LocalMatrixType(0, 0);
                LocalVectorType RHS_Contribution = LocalVectorType(0);

                //vector containing the localization in the system of the different terms
                Element::EquationIdVectorType EquationId;

                // assemble all elements
                for (int k = 0; k < nelements; k++)
                {
                    Model_Part::ElementsContainerType::iterator it = el_begin + k;

                    //detect if the element is active or not. If the user did not make any choice the element
                    //is active by default
                    bool element_is_active = true;
                    if ((it)->IsDefined(ACTIVE))
                        element_is_active = (it)->Is(ACTIVE);

                    if (element_is_active)
                    {
                        //calculate elemental contribution
                        pScheme->CalculateSystemContributions(*(it.base()), 
                                                                LHS_Contribution, 
                                                                RHS_Contribution, 
                                                                EquationId, 
                                                                rCurrentProcessInfo);

                        //assemble the elemental contribution
                        Assemble(rA, rb, LHS_Contribution, RHS_Contribution, EquationId);
                        // clean local elemental memory
                        pScheme->Clear(*(it.base()));
                    }

                }

                for (int k = 0; k < nconditions; k++)
                {
                    Model_Part::ConditionsContainerType::iterator it = cond_begin + k;

                    //detect if the condition is active or not. If the user did not make any choice the condition
                    //is active by default
                    bool condition_is_active = true;
                    if ((it)->IsDefined(ACTIVE))
                        condition_is_active = (it)->Is(ACTIVE);

                    if (condition_is_active)
                    {
                    //calculate elemental contribution
                    pScheme->Condition_CalculateSystemContributions(*(it.base()), 
                                                                    LHS_Contribution, 
                                                                    RHS_Contribution, 
                                                                    EquationId, 
                                                                    rCurrentProcessInfo);
                    //assemble the elemental contribution
                    Assemble(rA, rb, LHS_Contribution, RHS_Contribution, EquationId);

                    // clean local elemental memory
                    pScheme->Clear(*(it.base()));
                    }
                }
            }

            /**
             * @brief This is a call to the linear system solver
             */
            void SystemSolve(GlobalMatrixType& rA,
                            GlobalVectorType& rDx,
                            GlobalVectorType& rb) override
            {
                double norm_b;
                if (TSparseSpace::Size(rb) != 0)
                    norm_b = TSparseSpace::TwoNorm(rb);
                else
                    norm_b = 0.00;

                if (norm_b != 0.00)
                {
                    //do solve
                    this->mpLinearSystemSolver->Solve(rA, rDx, rb);
                }
                else
                    TSparseSpace::SetToZero(rDx);

                //prints informations about the current time
                if (this->mEchoLevel > 1)
                {
                    std::cout << *(this->mpLinearSystemSolver) << std::endl;
                }
            }

            /**
             * @brief Function to perform the building and solving phase at the same time.
             * @details It is ideally the fastest and safer function to use when it is possible to solve just after building
             */
            void BuildAndSolve(SchemePointerType pScheme,
                                Model_Part& rModelPart,
                                GlobalMatrixType& rA,
                                GlobalVectorType& rDx,
                                GlobalVectorType& rb) override
            {
                // double begin_time = OpenMPUtils::GetCurrentTime();
                Build(pScheme, rModelPart, rA, rb);
                // double end_time = OpenMPUtils::GetCurrentTime();

                // if (this->mEchoLevel > 1 && rModelPart.GetCommunicator().MyPID() == 0)
                //   KRATOS_INFO("system_build_time") << end_time - begin_time << std::endl;

                ApplyDirichletConditions(pScheme, rModelPart, rA, rDx, rb);

                if (this->mEchoLevel == 3)
                {
                std::cerr << "LHS before solve" << "Matrix = " << rA << std::endl;
                std::cerr << "Dx before solve"  << "Solution = " << rDx << std::endl;
                std::cerr << "RHS before solve" << "Vector = " << rb << std::endl;
                }

                // begin_time = OpenMPUtils::GetCurrentTime();
                SystemSolveWithPhysics(rA, rDx, rb, rModelPart);
                // end_time = OpenMPUtils::GetCurrentTime();


                // if (this->mEchoLevel > 1 && rModelPart.GetCommunicator().MyPID() == 0)
                //   KRATOS_INFO("system_solve_time") << end_time - begin_time << std::endl;

                if (this->mEchoLevel == 3)
                {
                std::cerr << "LHS after solve" << "Matrix = " << rA << std::endl;
                std::cerr << "Dx after solve"  << "Solution = " << rDx << std::endl;
                std::cerr <<"RHS after solve" << "Vector = " << rb << std::endl;
                }

            }

            /**
             * @brief Function to perform the building of the RHS and solving phase at the same time.
             * @details  It corresponds to the previews, but the System's matrix is considered already built and only the RHS is built again
             */
            void BuildRHSAndSolve(SchemePointerType pScheme,
                                    Model_Part& rModelPart,
                                    GlobalMatrixType& rA,
                                    GlobalVectorType& rDx,
                                    GlobalVectorType& rb) override
            {
                BuildRHS(pScheme, rModelPart, rb);
                SystemSolve(rA, rDx, rb);
            }


            /**
             * @brief applies the dirichlet conditions.
             * @details This operation may be very heavy or completely
             * @details unexpensive depending on the implementation choosen and on how the System Matrix
             * @details is built. For explanation of how it works for a particular implementation the user
             * @details should refer to the particular Builder And Solver choosen
             */
            void ApplyDirichletConditions(SchemePointerType pScheme,
                                            Model_Part& rModelPart,
                                            GlobalMatrixType& rA,
                                            GlobalVectorType& rDx,
                                            GlobalVectorType& rb) override
            {
                std::size_t system_size = rA.size1();
                std::vector<double> scaling_factors (system_size, 0.0f);

                const int ndofs = static_cast<int>(this->mDofSet.size());

                //NOTE: dofs are assumed to be numbered consecutively in the BlockBuilderAndSolver
                for(int k = 0; k<ndofs; k++)
                {
                    typename DofsArrayType::iterator dof_iterator = this->mDofSet.begin() + k;
                    if(dof_iterator->IsFixed())
                        scaling_factors[k] = 0.0f;
                    else
                        scaling_factors[k] = 1.0f;

                }

                double* Avalues = rA.value_data().begin();
                std::size_t* Arow_indices = rA.index1_data().begin();
                std::size_t* Acol_indices = rA.index2_data().begin();

                //detect if there is a line of all zeros and set the diagonal to a 1 if this happens
                for(int k = 0; k < static_cast<int>(system_size); ++k)
                {
                std::size_t col_begin = Arow_indices[k];
                std::size_t col_end = Arow_indices[k+1];
                bool empty = true;
                for (std::size_t j = col_begin; j < col_end; ++j)
                {
                    if(Avalues[j] != 0.0)
                    {
                    empty = false;
                    break;
                    }
                }

                if(empty == true)
                {
                    rA(k,k) = 1.0;
                    rb[k] = 0.0;
                }
                }

                for (int k = 0; k < static_cast<int>(system_size); ++k)
                {
                std::size_t col_begin = Arow_indices[k];
                std::size_t col_end = Arow_indices[k+1];
                double k_factor = scaling_factors[k];
                if (k_factor == 0)
                {
                    // zero out the whole row, except the diagonal
                    for (std::size_t j = col_begin; j < col_end; ++j)
                    if (static_cast<int>(Acol_indices[j]) != k )
                        Avalues[j] = 0.0;

                    // zero out the RHS
                    rb[k] = 0.0;
                }
                else
                {
                    // zero out the column which is associated with the zero'ed row
                    for (std::size_t j = col_begin; j < col_end; ++j)
                    if(scaling_factors[ Acol_indices[j] ] == 0 )
                        Avalues[j] = 0.0;
                }
                }
            }

            /**
             * @brief Builds the list of the DofSets involved in the problem by "asking" to each element and condition its Dofs.
             * @details The list of dofs is stores insde the BuilderAndSolver as it is closely connected to the way the matrix and RHS are built
             */
            void SetUpDofSet(SchemePointerType pScheme,
                            Model_Part& rModelPart) override
            {

                //Gets the array of elements from the modeler
                ElementsContainerType& rElements = rModelPart.Elements();
                const int nelements = static_cast<int>(rElements.size());

                Element::DofsVectorType ElementalDofList;

                Process_Info& rCurrentProcessInfo = rModelPart.GetProcessInfo();

                typedef std::unordered_set < Node::DofType::Pointer, dof_iterator_hash>  set_type;

                #ifdef OPENMP
                    int nthreads = omp_get_max_threads();
                #else
                    int nthreads = 1;
                #endif

                std::vector<set_type> dofs_aux_list(nthreads);

                if( this->mEchoLevel > 2)
                {
                    std::cout <<"setting_dofs" << "Number of threads:" << nthreads << std::endl;
                }

                for (int i = 0; i < static_cast<int>(nthreads); i++)
                {
                    dofs_aux_list[i].reserve(nelements);
                }

                if( this->mEchoLevel > 2)
                {
                    std::cout <<"setting_dofs" << "initialize_elements" << std::endl;
                }

                for (int i = 0; i < nelements; i++)
                {
                    typename ElementsContainerType::iterator it = rElements.begin() + i;
                    #ifdef OPENMP
                        const unsigned int this_thread_id = omp_get_thread_num();
                    #else
                        unsigned int this_thread_id = 0;
                    #endif

                    // gets list of Dof involved on every element
                    pScheme->GetElementalDofList(*(it.base()), ElementalDofList, rCurrentProcessInfo);

                    dofs_aux_list[this_thread_id].insert(ElementalDofList.begin(), ElementalDofList.end());
                }

                if( this->mEchoLevel > 2)
                {
                    std::cout << "setting_dofs" << "initialize_conditions" << std::endl;
                }

                ConditionsContainerType& rConditions = rModelPart.Conditions();
                const int nconditions = static_cast<int>(rConditions.size());
                for (int i = 0; i < nconditions; i++)
                {
                    typename ConditionsContainerType::iterator it = rConditions.begin() + i;
                    #ifdef OPENMP
                        const unsigned int this_thread_id = omp_get_thread_num();
                    #else
                        unsigned int this_thread_id = 0;
                    #endif

                    // gets list of Dof involved on every condition
                    pScheme->GetConditionDofList(*(it.base()), ElementalDofList, rCurrentProcessInfo);
                    dofs_aux_list[this_thread_id].insert(ElementalDofList.begin(), ElementalDofList.end());

                }

                if( this->mEchoLevel > 2)
                {
                    std::cout << "setting_dofs" << "initialize tree reduction" << std::endl;
                }

                // Here we do a reduction in a tree so to have everything on thread 0
                unsigned int old_max = nthreads;
                unsigned int new_max = ceil(0.5*static_cast<double>(old_max));
                while (new_max>=1 && new_max != old_max)
                {
                    if( this->mEchoLevel > 2)
                    {
                        //just for debugging
                        std::cout << "setting_dofs" << "old_max" << old_max << " new_max:" << new_max << std::endl;
                        for (int i = 0; i < static_cast<int>(new_max); i++)
                        {
                        if (i + new_max < old_max)
                        {
                            std::cout << "setting_dofs" << i << " - " << i+new_max << std::endl;
                        }
                        }
                    }
                    for (int i = 0; i < static_cast<int>(new_max); i++)
                    {
                        if (i + new_max < old_max)
                        {
                        dofs_aux_list[i].insert(dofs_aux_list[i+new_max].begin(), dofs_aux_list[i+new_max].end());
                        dofs_aux_list[i+new_max].clear();
                        }
                    }
                    old_max = new_max;
                    new_max = ceil(0.5*static_cast<double>(old_max));
                }

                if( this->mEchoLevel > 2)
                {
                    std::cout << "setting_dofs" << "initializing ordered array filling" << std::endl;
                }

                DofsArrayType Doftemp;
                this->mDofSet = DofsArrayType();

                Doftemp.reserve(dofs_aux_list[0].size());
                for (auto it= dofs_aux_list[0].begin(); it!= dofs_aux_list[0].end(); it++)
                {
                    Doftemp.push_back( *it );
                }
                Doftemp.Sort();
                this->mDofSet = Doftemp;

                //Throws an exception if there are no Degrees Of Freedom involved in the analysis
                if (this->mDofSet.size() == 0)
                {
                    std::cerr << "No degrees of freedom!" << std::endl;
                    exit(0);
                }
                if( this->mEchoLevel > 2)
                {
                    std::cout << "Dofs size" << this->mDofSet.size() << std::endl;
                }


                if( this->mEchoLevel > 2)
                {
                    std::cout << "setting_dofs" << "Initializing lock array" << std::endl;
                }

                if( this->mEchoLevel > 2)
                {
                    std::cout << "setting_dofs" << "End of setupdofset" << std::endl;
                }

                this->Set(LocalFlagType::DOFS_INITIALIZED, true);
            }

            /**
             * @brief organises the dofset in order to speed up the building phase
             */
            void SetUpSystem() override
            {
                //int free_id = 0;
                this->mEquationSystemSize = this->mDofSet.size();
                int ndofs = static_cast<int>(this->mDofSet.size());
                for (int i = 0; i < static_cast<int>(ndofs); i++)
                {
                    typename DofsArrayType::iterator dof_iterator = this->mDofSet.begin() + i;
                    dof_iterator->SetEquationId(i);
                }
            }

            /**
             * @brief Resizes and Initializes the system vectors and matrices after SetUpDofSet and SetUpSytem has been called
             */
            void SetUpSystemMatrices(SchemePointerType pScheme,
                                    Model_Part& rModelPart,
                                    GlobalMatrixPointerType& pA,
                                    GlobalVectorPointerType& pDx,
                                    GlobalVectorPointerType& pb) override
            {
                if (pA == nullptr) //if the pointer is not initialized initialize it to an empty matrix
                {
                    GlobalMatrixPointerType pNewA = make_shared<GlobalMatrixType>(0, 0);
                    pA.swap(pNewA);
                }
                if (pDx == nullptr) //if the pointer is not initialized initialize it to an empty matrix
                {
                    GlobalVectorPointerType pNewDx = make_shared<GlobalVectorType>(0);
                    pDx.swap(pNewDx);
                }
                if (pb == nullptr) //if the pointer is not initialized initialize it to an empty matrix
                {
                    GlobalVectorPointerType pNewb = make_shared<GlobalVectorType>(0);
                    pb.swap(pNewb);
                }
                GlobalMatrixType& rA = *pA;
                GlobalVectorType& rDx = *pDx;
                GlobalVectorType& rb = *pb;
                //resizing the system vectors and matrix
                if (rA.size1() == 0 || this->mOptions.Is(LocalFlagType::REFORM_DOFS)) //if the matrix is not initialized
                {
                    rA.resize(this->mEquationSystemSize, this->mEquationSystemSize, false);
                    ConstructMatrixStructure(pScheme, rA, rModelPart.Elements(), rModelPart.Conditions(), rModelPart.GetProcessInfo());
                }
                else
                {
                    if (rA.size1() != this->mEquationSystemSize || rA.size2() != this->mEquationSystemSize)
                    {
                        std::cout << "block builder resize" << "it should not come here -> this is SLOW" << std::endl;
                        rA.resize(this->mEquationSystemSize, this->mEquationSystemSize, true);
                        ConstructMatrixStructure(pScheme, rA, rModelPart.Elements(), rModelPart.Conditions(), rModelPart.GetProcessInfo());
                    }
                }
                if (rDx.size() != this->mEquationSystemSize)
                rDx.resize(this->mEquationSystemSize, false);
                if (rb.size() != this->mEquationSystemSize)
                rb.resize(this->mEquationSystemSize, false);
            }


            /**
            * @brief Performs all the required operations that should be done (for each step) before solving the solution step.
            * @details this function must be called only once per step.
            */
            void InitializeSolutionStep(SchemePointerType pScheme,
                                        Model_Part& rModelPart,
                                        GlobalMatrixPointerType& pA,
                                        GlobalVectorPointerType& pDx,
                                        GlobalVectorPointerType& pb) override
            {
                BaseType::InitializeSolutionStep(pScheme, rModelPart, pA, pDx, pb);
            }

            /**
            * @brief Performs all the required operations that should be done (for each step) after solving the solution step.
            * @details this function must be called only once per step.
            */
            void FinalizeSolutionStep(SchemePointerType pScheme,
                                        Model_Part& rModelPart,
                                        GlobalMatrixPointerType& pA,
                                        GlobalVectorPointerType& pDx,
                                        GlobalVectorPointerType& pb) override
            {
                BaseType::FinalizeSolutionStep(pScheme, rModelPart, pA, pDx, pb);
            }

            /**
             * @brief Calculates system reactions
             * @details A flag controls if reactions must be calculated
             * @details An internal variable to store the reactions vector is needed
             */
            void CalculateReactions(SchemePointerType pScheme,
                                    Model_Part& rModelPart,
                                    GlobalMatrixType& rA,
                                    GlobalVectorType& rDx,
                                    GlobalVectorType& rb) override
            {
                TSparseSpace::SetToZero(rb);
                //refresh RHS to have the correct reactions
                BuildRHSNoDirichlet(pScheme, rModelPart, rb);
                const int ndofs = static_cast<int>(this->mDofSet.size());
                //NOTE: dofs are assumed to be numbered consecutively in the BlockBuilderAndSolver
                for (int k = 0; k<ndofs; k++)
                {
                    typename DofsArrayType::iterator dof_iterator = this->mDofSet.begin() + k;
                    const int i = (dof_iterator)->EquationId();
                    if ( (dof_iterator)->IsFixed() ) {
                        (dof_iterator)->GetSolutionStepReactionValue() = -rb[i];
                    } else {
                        (dof_iterator)->GetSolutionStepReactionValue() = 0.0;
                    }
                }
            }

            /**
             * @name Utility Operations
            */

            /**
             * @brief This function is intended to be called at the end of the solution step to clean up memory storage not needed
             */
            void Clear() override
            {
                BaseType::Clear();
            }
            /**
             * This function is designed to be called once to perform all the checks needed
             * on the input provided. Checks can be "expensive" as the function is designed
             * to catch user's errors.
             * @param rModelPart
             * @return 0 all ok
             */
            int Check(Model_Part& r_mUSE_GOOGLE_HASHodel_part) override
            {
                return 0;
            }
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
            void SystemSolveWithPhysics(GlobalMatrixType& rA,
                                GlobalVectorType& rDx,
                                GlobalVectorType& rb,
                                Model_Part& rModelPart)
            {
                double norm_b;
                if (TSparseSpace::Size(rb) != 0)
                    norm_b = TSparseSpace::TwoNorm(rb);
                else
                    norm_b = 0.00;

                if (norm_b != 0.00)
                {
                    //provide physical data as needed
                    if(this->mpLinearSystemSolver->AdditionalPhysicalDataIsNeeded() )
                        this->mpLinearSystemSolver->ProvideAdditionalData(rA, rDx, rb, this->mDofSet, rModelPart);

                    //do solve
                    this->mpLinearSystemSolver->Solve(rA, rDx, rb);
                }
                else
                {
                    TSparseSpace::SetToZero(rDx);
                    std::cout << "RHS" << "ATTENTION! setting the RHS to zero!" << std::endl;
                }

                //prints informations about the current time
                if (this->mEchoLevel > 1)
                {
                    std::cout << "LinearSolver" << *(this->mpLinearSystemSolver) << std::endl;
                }
            }

            virtual void ConstructMatrixStructure(SchemePointerType pScheme,
                                        GlobalMatrixType& rA,
                                        ElementsContainerType& rElements,
                                        ConditionsContainerType& rConditions,
                                        Process_Info& rCurrentProcessInfo)
            {
                //filling with zero the matrix (creating the structure)
                // double begin_time = OpenMPUtils::GetCurrentTime();

                const std::size_t equation_size = this->mEquationSystemSize;

                std::vector<std::unordered_set<std::size_t> > indices(equation_size);

                for (int iii = 0; iii < static_cast<int>(equation_size); iii++)
                {
                    indices[iii].reserve(40);
                }

                Element::EquationIdVectorType ids(3, 0);

                const int nelements = static_cast<int>(rElements.size());
                for(int iii=0; iii<nelements; iii++)
                {
                    typename ElementsContainerType::iterator i_element = rElements.begin() + iii;
                    pScheme->EquationId( *(i_element.base()) , ids, rCurrentProcessInfo);
                    for (std::size_t i = 0; i < ids.size(); i++)
                    {
                        auto& row_indices = indices[ids[i]];
                        row_indices.insert(ids.begin(), ids.end());
                    }
                }

                const int nconditions = static_cast<int>(rConditions.size());
                for (int iii = 0; iii<nconditions; iii++)
                {
                    typename ConditionsContainerType::iterator i_condition = rConditions.begin() + iii;
                    pScheme->Condition_EquationId( *(i_condition.base()), ids, rCurrentProcessInfo);
                    for (std::size_t i = 0; i < ids.size(); i++)
                    {
                        auto& row_indices = indices[ids[i]];
                        row_indices.insert(ids.begin(), ids.end());
                    }
                }

                //count the row sizes
                unsigned int nnz = 0;
                for (unsigned int i = 0; i < indices.size(); i++)
                nnz += indices[i].size();

                rA = GlobalMatrixType(indices.size(), indices.size());

                double* Avalues = rA.value_data().begin();
                std::size_t* Arow_indices = rA.index1_data().begin();
                std::size_t* Acol_indices = rA.index2_data().begin();

                //filling the index1 vector - DO NOT MAKE PARALLEL THE FOLLOWING LOOP!
                Arow_indices[0] = 0;
                for (int i = 0; i < static_cast<int>(rA.size1()); i++)
                Arow_indices[i+1] = Arow_indices[i] + indices[i].size();


                for (int i = 0; i < static_cast<int>(rA.size1()); i++)
                {
                    const unsigned int row_begin = Arow_indices[i];
                    const unsigned int row_end = Arow_indices[i+1];
                    unsigned int k = row_begin;
                    for (auto it = indices[i].begin(); it != indices[i].end(); it++)
                    {
                        Acol_indices[k] = *it;
                        Avalues[k] = 0.0;
                        k++;
                    }
                    indices[i].clear(); //deallocating the memory
                    std::sort(&Acol_indices[row_begin], &Acol_indices[row_end]);
                }

                rA.set_filled(indices.size()+1, nnz);
            }
            void AssembleLHS(GlobalMatrixType& rA,
                    LocalMatrixType& rLHS_Contribution,
                    Element::EquationIdVectorType& rEquationId)
            {
                unsigned int local_size = rLHS_Contribution.size1();
                for (unsigned int i_local = 0; i_local < local_size; i_local++)
                {
                    unsigned int i_global = rEquationId[i_local];
                    for (unsigned int j_local = 0; j_local < local_size; j_local++)
                    {
                        unsigned int j_global = rEquationId[j_local];
                        rA(i_global, j_global) += rLHS_Contribution(i_local, j_local);
                    }
                }

            }

            void AssembleRHS(GlobalVectorType& rb,
                            LocalVectorType& rRHS_Contribution,
                            Element::EquationIdVectorType& rEquationId)
            {
                unsigned int local_size = rRHS_Contribution.size();
                for (unsigned int i_local = 0; i_local < local_size; i_local++)
                {
                    unsigned int i_global = rEquationId[i_local];
                    // ASSEMBLING THE SYSTEM VECTOR
                    double& b_value = rb[i_global];
                    const double& rhs_value = rRHS_Contribution[i_local];
                    b_value += rhs_value;
                }
            }

            void Assemble(GlobalMatrixType& rA,
                            GlobalVectorType& rb,
                            const LocalMatrixType& rLHS_Contribution,
                            const LocalVectorType& rRHS_Contribution,
                            Element::EquationIdVectorType& rEquationId
                            )
            {
                unsigned int local_size = rLHS_Contribution.size1();

                for (unsigned int i_local = 0; i_local < local_size; i_local++)
                {
                    unsigned int i_global = rEquationId[i_local];
                    double& r_a = rb[i_global];
                    const double& v_a = rRHS_Contribution(i_local);
                    r_a += v_a;
                    AssembleRowContribution(rA, rLHS_Contribution, i_global, i_local, rEquationId);
                }
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
            Flags mOptions;

        /// @}


        /// @name Private Operatiors
        /// @{


        /// @}


        /// @name Private Operations
        /// @{
            inline void AddUnique(std::vector<std::size_t>& v, const std::size_t& candidate)
            {
                std::vector<std::size_t>::iterator i = v.begin();
                std::vector<std::size_t>::iterator endit = v.end();
                while (i != endit && (*i) != candidate)
                {
                    ++i;
                }
                if (i == endit)
                {
                    v.push_back(candidate);
                }
            }

            void BuildRHSNoDirichlet(SchemePointerType pScheme,
                            Model_Part& rModelPart,
                            GlobalVectorType& rb)
            {

                //Getting the Elements
                ElementsContainerType& rElements = rModelPart.Elements();

                //getting the array of the conditions
                ConditionsContainerType& ConditionsArray = rModelPart.Conditions();

                Process_Info& rCurrentProcessInfo = rModelPart.GetProcessInfo();

                //contributions to the system
                LocalMatrixType LHS_Contribution = LocalMatrixType(0, 0);
                LocalVectorType RHS_Contribution = LocalVectorType(0);

                //vector containing the localization in the system of the different
                //terms
                Element::EquationIdVectorType EquationId;

                // assemble all elements
                //for (typename ElementsContainerType::ptr_iterator it = rElements.ptr_begin(); it != rElements.ptr_end(); ++it)

                const int nelements = static_cast<int>(rElements.size());
                for(int i=0; i<nelements; i++)
                {
                    typename ElementsContainerType::iterator it = rElements.begin() + i;
                    //detect if the element is active or not. If the user did not make any choice the element
                    //is active by default
                    bool element_is_active = true;
                    if( (it)->IsDefined(ACTIVE) )
                        element_is_active = (it)->Is(ACTIVE);
                    if(element_is_active)
                    {
                        //calculate elemental Right Hand Side Contribution
                        pScheme->Calculate_RHS_Contribution(*(it.base()), RHS_Contribution, EquationId, rCurrentProcessInfo);
                        //assemble the elemental contribution
                        AssembleRHS(rb, RHS_Contribution, EquationId);
                    }
                }

                LHS_Contribution.resize(0, 0, false);
                RHS_Contribution.resize(0, false);

                // assemble all conditions
                //for (typename ConditionsContainerType::ptr_iterator it = ConditionsArray.ptr_begin(); it != ConditionsArray.ptr_end(); ++it)
                const int nconditions = static_cast<int>(ConditionsArray.size());
                //#pragma omp parallel for firstprivate(nconditions, RHS_Contribution, EquationId) schedule(dynamic, 1024)
                for (int i = 0; i<nconditions; i++)
                {
                    auto it = ConditionsArray.begin() + i;
                    //detect if the element is active or not. If the user did not make any choice the element
                    //is active by default
                    bool condition_is_active = true;
                    if( (it)->IsDefined(ACTIVE) )
                        condition_is_active = (it)->Is(ACTIVE);

                    if(condition_is_active)
                    {
                        //calculate elemental contribution
                        pScheme->Condition_Calculate_RHS_Contribution(*(it.base()), RHS_Contribution, EquationId, rCurrentProcessInfo);
                        //assemble the elemental contribution
                        AssembleRHS(rb, RHS_Contribution, EquationId);
                    }
                }
            }


            inline void CreatePartition(unsigned int number_of_threads,
                                        const int number_of_rows,
                                        std::vector<unsigned int>& partitions)
            {
                partitions.resize(number_of_threads + 1);
                int partition_size = number_of_rows / number_of_threads;
                partitions[0] = 0;
                partitions[number_of_threads] = number_of_rows;
                for (unsigned int i = 1; i < number_of_threads; i++)
                    partitions[i] = partitions[i - 1] + partition_size;
            }

            inline void AssembleRowContribution(GlobalMatrixType& rA,
                                                const LocalMatrixType& rAlocal,
                                                const unsigned int i,
                                                const unsigned int i_local,
                                                Element::EquationIdVectorType& rEquationId)
            {
                double* values_vector = rA.value_data().begin();
                std::size_t* index1_vector = rA.index1_data().begin();
                std::size_t* index2_vector = rA.index2_data().begin();

                size_t left_limit = index1_vector[i];
                //	size_t right_limit = index1_vector[i+1];

                //find the first entry
                size_t last_pos = ForwardFind(rEquationId[0],left_limit,index2_vector);
                size_t last_found = rEquationId[0];

                double& r_a = values_vector[last_pos];
                const double& v_a = rAlocal(i_local,0);
                r_a +=  v_a;

                //now find all of the other entries
                size_t pos = 0;
                for(unsigned int j=1; j<rEquationId.size(); j++)
                {
                    unsigned int id_to_find = rEquationId[j];
                    if(id_to_find > last_found)
                        pos = ForwardFind(id_to_find,last_pos+1,index2_vector);
                    else
                        pos = BackwardFind(id_to_find,last_pos-1,index2_vector);

                    double& r = values_vector[pos];
                    const double& v = rAlocal(i_local,j);
                    r +=  v;

                    last_found = id_to_find;
                    last_pos = pos;
                }
            }

            inline unsigned int ForwardFind(const unsigned int id_to_find,
                                            const unsigned int start,
                                            const size_t* index_vector)
            {
                unsigned int pos = start;
                while(id_to_find != index_vector[pos]) pos++;
                return pos;
            }

            inline unsigned int BackwardFind(const unsigned int id_to_find,
                                            const unsigned int start,
                                            const size_t* index_vector)
            {
                unsigned int pos = start;
                while(id_to_find != index_vector[pos]) pos--;
                return pos;
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