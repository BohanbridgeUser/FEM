#ifndef _SOLUTION_SCHEME_H_
#define _SOLUTION_SCHEME_H_

#include "../../Model/model_part.h"
#include "../../Process/solver_process.h"
#include "../solve_local_flags.h"
#include "../time_integration_methods/time_integration_method.h"

/** 
 * @brief Solution scheme base class
 *  @details This is the base class for the schemes
 */
template<class TSparseSpace,class TDenseSpace>
class Solution_Scheme : public Flags
{
 public:

  ///@name Type Definitions
  ///@{
  typedef Solution_Scheme<TSparseSpace,TDenseSpace>                SolutionSchemeType;
  typedef typename SolutionSchemeType::Pointer              SolutionSchemePointerType;
  typedef Solver_Local_Flags                                            LocalFlagType;
  typedef typename ModelPart::DofsArrayType                             DofsArrayType;

  typedef typename TSparseSpace::SparseMatrix                        SystemMatrixType;
  typedef typename TSparseSpace::SparseVector                        SystemVectorType;
  typedef typename TDenseSpace::Matrix                          LocalSystemMatrixType;
  typedef typename TDenseSpace::Vector                          LocalSystemVectorType;

  typedef ModelPart::NodesContainerType                            NodesContainerType;
  typedef ModelPart::ElementsContainerType                      ElementsContainerType;
  typedef ModelPart::ConditionsContainerType                  ConditionsContainerType;

  typedef ModelPart::NodeType                                                NodeType;
  typedef std::array<double, 3>                                            VectorType;
  typedef Variable<VectorType>                                     VariableVectorType;
  typedef Time_Integration_Method<VariableVectorType,VectorType> IntegrationVectorType;
  typedef typename IntegrationVectorType::Pointer        IntegrationVectorPointerType;
  typedef std::vector<IntegrationVectorPointerType>      IntegrationMethodsVectorType;

  typedef Variable<double>                                         VariableScalarType;
  typedef Time_Integration_Method<VariableScalarType, double>    IntegrationScalarType;
  typedef typename IntegrationScalarType::Pointer        IntegrationScalarPointerType;
  typedef std::vector<IntegrationScalarPointerType>      IntegrationMethodsScalarType;

  typedef typename Solver_Process::Pointer                         ProcessPointerType;
  typedef std::vector<ProcessPointerType>                    ProcessPointerVectorType;

  /// Pointer definition of Solution_Scheme
  LOTUS_SHARED_POINTER_DEFINE(Solution_Scheme);

  ///@}
  ///@name Life Cycle
  ///@{

  /// Default Constructor.
  Solution_Scheme() : Flags() {SetDefaultFlags();}

  /// Constructor.
  Solution_Scheme(Flags& rOptions) : Flags(), mOptions(rOptions) {SetDefaultFlags();}

  /// Constructor.
  Solution_Scheme(IntegrationMethodsVectorType& rTimeVectorIntegrationMethods, Flags& rOptions)
  : Flags(), 
  mOptions(rOptions), 
  mTimeVectorIntegrationMethods(rTimeVectorIntegrationMethods) 
  {SetDefaultFlags();}

  /// Constructor.
  Solution_Scheme(IntegrationMethodsVectorType& rTimeVectorIntegrationMethods)
  : Flags(), 
  mTimeVectorIntegrationMethods(rTimeVectorIntegrationMethods) 
  {SetDefaultFlags();}

  /// Constructor.
  Solution_Scheme(IntegrationMethodsScalarType& rTimeScalarIntegrationMethods, Flags& rOptions) 
  : Flags(), 
  mOptions(rOptions), 
  mTimeScalarIntegrationMethods(rTimeScalarIntegrationMethods) 
  {SetDefaultFlags();}

  /// Constructor.
  Solution_Scheme(IntegrationMethodsScalarType& rTimeScalarIntegrationMethods) 
  : Flags(), 
  mTimeScalarIntegrationMethods(rTimeScalarIntegrationMethods) 
  {SetDefaultFlags();}

  /// Constructor.
  Solution_Scheme(IntegrationMethodsVectorType& rTimeVectorIntegrationMethods,
                 IntegrationMethodsScalarType& rTimeScalarIntegrationMethods,
                 Flags& rOptions)
  : Flags(), 
  mOptions(rOptions), 
  mTimeVectorIntegrationMethods(rTimeVectorIntegrationMethods), 
  mTimeScalarIntegrationMethods(rTimeScalarIntegrationMethods) 
  {SetDefaultFlags();}

  /// Constructor.
  Solution_Scheme(IntegrationMethodsVectorType& rTimeVectorIntegrationMethods,
                 IntegrationMethodsScalarType& rTimeScalarIntegrationMethods)
  : Flags(), 
  mTimeVectorIntegrationMethods(rTimeVectorIntegrationMethods), 
  mTimeScalarIntegrationMethods(rTimeScalarIntegrationMethods) 
  {SetDefaultFlags();}

  /// Copy contructor.
  Solution_Scheme(Solution_Scheme& rOther) 
  : mOptions(rOther.mOptions), 
  mProcesses(rOther.mProcesses)
  {
    std::copy(std::begin(rOther.mTimeVectorIntegrationMethods), std::end(rOther.mTimeVectorIntegrationMethods), std::back_inserter(mTimeVectorIntegrationMethods));
  }

  /// Clone.
  virtual SolutionSchemePointerType Clone()
  {
    return SolutionSchemePointerType( new Solution_Scheme(*this) );
  }

  /// Destructor.
  ~Solution_Scheme() override {}

  ///@}
  ///@name Operators
  ///@{

  ///@}
  ///@name Operations
  ///@{

  /**
   * @brief SetDefaultSchemeFlags.
   * @details This is intended to be called with the constructor
   */
  void SetDefaultFlags()
  {
    if( this->mOptions.IsNotDefined(LocalFlagType::MOVE_MESH) )
      mOptions.Set(LocalFlagType::MOVE_MESH,true); //default : lagrangian mesh update

    if( this->mOptions.IsNotDefined(LocalFlagType::UPDATE_VARIABLES) )
      mOptions.Set(LocalFlagType::UPDATE_VARIABLES,true); //default : derivatives update

    if( this->mOptions.IsNotDefined(LocalFlagType::INCREMENTAL_SOLUTION) )
      mOptions.Set(LocalFlagType::INCREMENTAL_SOLUTION,true); //default : dof is the variable increment
  }

  /**
   * @brief Performs all the required operations that should be done (for each step) before solving the solution step.
   * @details This is intended to be called just once when the strategy is initialized
   */
  virtual void Initialize(ModelPart& rModelPart)
  {
    

    for(typename IntegrationMethodsVectorType::iterator it=mTimeVectorIntegrationMethods.begin();
        it!=mTimeVectorIntegrationMethods.end(); ++it)
      (*it)->SetParameters(rModelPart.GetProcessInfo());

    for(typename IntegrationMethodsScalarType::iterator it=mTimeScalarIntegrationMethods.begin();
        it!=mTimeScalarIntegrationMethods.end(); ++it)
      (*it)->SetParameters(rModelPart.GetProcessInfo());

    this->InitializeElements(rModelPart);

    this->InitializeConditions(rModelPart);

    this->Set(LocalFlagType::INITIALIZED, true);

    
  }

  /**
   * @brief Performs all the required operations that should be done (for each step) before solving the solution step.
   * @details this function must be called only once per step.
   */
  virtual void InitializeSolutionStep(ModelPart& rModelPart)
  {
    

    Process_Info& rCurrentProcessInfo = rModelPart.GetProcessInfo();

    for(typename ProcessPointerVectorType::iterator it=mProcesses.begin(); it!=mProcesses.end(); ++it)
      (*it)->ExecuteInitializeSolutionStep();

//#pragma omp parallel for
    for(int i=0; i<static_cast<int>(rModelPart.Elements().size()); i++)
    {
      auto itElem = rModelPart.ElementsBegin() + i;
      itElem->InitializeSolutionStep(rCurrentProcessInfo);
    }

//#pragma omp parallel for
    for(int i=0; i<static_cast<int>(rModelPart.Conditions().size()); i++)
    {
      auto itCond = rModelPart.ConditionsBegin() + i;
      itCond->InitializeSolutionStep(rCurrentProcessInfo);
    }

  }


  /**
   * @brief Performs all the required operations that should be done (for each step) after solving the solution step.
   * @details this function must be called only once per step.
   */

  virtual void FinalizeSolutionStep(ModelPart& rModelPart)
  {
    

    Process_Info& rCurrentProcessInfo = rModelPart.GetProcessInfo();

    for(typename ProcessPointerVectorType::iterator it=mProcesses.begin(); it!=mProcesses.end(); ++it)
      (*it)->ExecuteFinalizeSolutionStep();


//#pragma omp parallel for
    for(int i=0; i<static_cast<int>(rModelPart.Elements().size()); i++)
    {
      auto itElem = rModelPart.ElementsBegin() + i;
      itElem->FinalizeSolutionStep(rCurrentProcessInfo);
    }


//#pragma omp parallel for
    for(int i=0; i<static_cast<int>(rModelPart.Conditions().size()); i++)
    {
      auto itCond = rModelPart.ConditionsBegin() + i;
      itCond->FinalizeSolutionStep(rCurrentProcessInfo);
    }

    
  }

  /**
   * @brief Performs all the required operations that should be done (for each iteration) before solving a solution iteration.
   * @details this function must be called only once per iteration
   */
  virtual void InitializeNonLinearIteration(ModelPart& rModelPart)
  {
    

    Process_Info& rCurrentProcessInfo = rModelPart.GetProcessInfo();

    for(typename ProcessPointerVectorType::iterator it=mProcesses.begin(); it!=mProcesses.end(); ++it)
      (*it)->ExecuteInitialize(); //corresponds to ExecuteInitializeNonLinearIteration()

//#pragma omp parallel for
    for(int i=0; i<static_cast<int>(rModelPart.Elements().size()); i++)
    {
      auto itElem = rModelPart.ElementsBegin() + i;
      itElem->InitializeNonLinearIteration(rCurrentProcessInfo);
    }


//#pragma omp parallel for
    for(int i=0; i<static_cast<int>(rModelPart.Conditions().size()); i++)
    {
      auto itCond = rModelPart.ConditionsBegin() + i;
      itCond->InitializeNonLinearIteration(rCurrentProcessInfo);
    }


    
  }


  /**
   * @brief Performs all the required operations that should be done (for each iteration) after solving a solution iteration.
   * @details this function must be called only once per iteration
   */
  virtual void FinalizeNonLinearIteration(ModelPart& rModelPart)
  {
    

    for(typename ProcessPointerVectorType::iterator it=mProcesses.begin(); it!=mProcesses.end(); ++it)
      (*it)->ExecuteFinalize(); //corresponds to ExecuteFinalizeNonLinearIteration()

    Process_Info& rCurrentProcessInfo = rModelPart.GetProcessInfo();

//#pragma omp parallel for
    for(int i=0; i<static_cast<int>(rModelPart.Elements().size()); i++)
    {
      auto itElem = rModelPart.ElementsBegin() + i;
      itElem->FinalizeNonLinearIteration(rCurrentProcessInfo);
    }


//#pragma omp parallel for
    for(int i=0; i<static_cast<int>(rModelPart.Conditions().size()); i++)
    {
      auto itCond = rModelPart.ConditionsBegin() + i;
      itCond->FinalizeNonLinearIteration(rCurrentProcessInfo);
    }

    
  }


  /**
   * @brief Performing the prediction of the solution.
   * @details this function must be called only once per step.
   */
  virtual void Predict(ModelPart& rModelPart,
                       DofsArrayType& rDofSet,
                       SystemVectorType& rDx)
  {
    

    
  }

  /**
   * @brief Performing the update of the solution.
   * @details this function must be called only once per iteration
   */
  virtual void Update(ModelPart& rModelPart,
                      DofsArrayType& rDofSet,
                      SystemVectorType& rDx)
  {

  }


  /**
   * @brief Performing the update of the solution Dofs (total solution)
   * @details this function must be called only once per iteration
   */
  static inline void SetSolution(ModelPart& rModelPart,
                                 DofsArrayType& rDofSet,
                                 SystemVectorType& rDx)
  {
    #ifndef _OPENMP
      const unsigned int NumThreads = 1;
    #else
      const unsigned int NumThreads = omp_get_num_threads();
    #endif

    std::vector<int> DofPartition;
    DofPartition[0] = 0;
    DofPartition[1] = DofPartition[0] + rDofSet.size();

    const int ndof = static_cast<int>(rDofSet.size());
    typename DofsArrayType::iterator DofBegin = rDofSet.begin();

    //#pragma omp parallel for firstprivate(DofBegin)
    for(int i = 0;  i < ndof; i++)
    {
        typename DofsArrayType::iterator itDof = DofBegin + i;

        if (itDof->IsFree() )
        {
          itDof->GetSolutionStepValue() = rDx.coeffRef(itDof->EquationId());
        }
    }
  }


  /**
   * @brief Performing the update of the solution Dofs (incremental solution)
   * @details this function must be called only once per iteration
   */
  static inline void AddSolution(ModelPart& rModelPart,
                                 DofsArrayType& rDofSet,
                                 SystemVectorType& rDx)
  {
    
    #ifndef _OPENMP
      const unsigned int NumThreads = 1;
    #else
      const unsigned int NumThreads = omp_get_num_threads();
    #endif

    // Update of displacement (by DOF)
    std::vector<int> DofPartition(NumThreads+1);
    DofPartition[0] = 0;
    DofPartition[NumThreads] = DofPartition[0] + rDofSet.size();

    const int ndof = static_cast<int>(rDofSet.size());
    typename DofsArrayType::iterator DofBegin = rDofSet.begin();

    //#pragma omp parallel for firstprivate(DofBegin)
    for(int i = 0;  i < ndof; i++)
    {
      typename DofsArrayType::iterator itDof = DofBegin + i;

      if (itDof->IsFree() )
      {
        itDof->GetSolutionStepValue() += rDx.coeffRef(itDof->EquationId());
      }
    }
  }

  /**
   * @brief Performing the update of the solution Dofs
   * @details this function must be called only once per iteration
   */
  virtual void UpdateDofs(ModelPart& rModelPart,
                          DofsArrayType& rDofSet,
                          SystemVectorType& rDx)
  {
    if( mOptions.Is(LocalFlagType::INCREMENTAL_SOLUTION) )
      AddSolution(rModelPart,rDofSet,rDx);  //dof = incremental variable
    else
      SetSolution(rModelPart,rDofSet,rDx);  //dof = total variable
  }



  /**
   * @brief Performing the update of the solution variables
   * @details this function must be called only once per iteration
   */
  virtual void UpdateVariables(ModelPart& rModelPart)
  {
    if( this->mOptions.Is(LocalFlagType::UPDATE_VARIABLES) ){

      #ifndef _OPENMP
        const unsigned int NumThreads = 1;
      #else
        const unsigned int NumThreads = omp_get_num_threads();
      #endif

      std::vector<int> NodePartition(NumThreads+1);
      NodePartition[0] = 0;
      NodePartition[NumThreads] = NodePartition[0] + rModelPart.Nodes().size();

      const int nnodes = static_cast<int>(rModelPart.Nodes().size());
      NodesContainerType::iterator NodeBegin = rModelPart.Nodes().begin();

      //#pragma omp parallel for firstprivate(NodeBegin)
      for(int i = 0;  i < nnodes; i++)
      {
        NodesContainerType::iterator itNode = NodeBegin + i;
        this->IntegrationMethodUpdate(*itNode);
      }
    }
  }

  /**
   * @brief Performing the prediction of the solution variables
   * @details this function must be called only once per iteration
   */
  virtual void PredictVariables(ModelPart& rModelPart)
  {
    #ifndef _OPENMP
      const unsigned int NumThreads = 1;
    #else
      const unsigned int NumThreads = 1;// omp_get_num_threads();
    #endif

    std::vector<int> NodePartition(NumThreads+1);
    NodePartition[0] = 0;
    NodePartition[NumThreads] = NodePartition[0] + rModelPart.Nodes().size();

    const int nnodes = static_cast<int>(rModelPart.Nodes().size());
    NodesContainerType::iterator NodeBegin = rModelPart.Nodes().begin();

    //#pragma omp parallel for firstprivate(NodeBegin)
    for(int i = 0;  i < nnodes; i++)
    {
      NodesContainerType::iterator itNode = NodeBegin + i;

      this->IntegrationMethodPredict(*itNode);
    }
  }


  /**
   * @brief This function is designed to move the mesh
   * @note Be careful it just consider displacements, derive this method to adapt to your own strategies (ALE, FSI, etc...)
   */
  virtual void MoveMesh(ModelPart& rModelPart)
  {
    

    if( this->mOptions.Is(LocalFlagType::MOVE_MESH) ){

      if (rModelPart.NodesBegin()->SolutionStepsDataHas(DISPLACEMENT_X) == false)
      {
        std::cerr << "It is impossible to move the mesh since the DISPLACEMENT variable is not in the Model Part. Add DISPLACEMENT to the list of variables" << std::endl;
        exit(0);
      }

      bool DisplacementIntegration = false;
      for(typename IntegrationMethodsVectorType::iterator it=mTimeVectorIntegrationMethods.begin();
          it!=mTimeVectorIntegrationMethods.end(); ++it)
      {
        if( "DISPLACEMENT" == (*it)->GetVariableName() ){
          DisplacementIntegration = true;
          break;
        }
      }

      if(DisplacementIntegration == true){

        // Update mesh positions : node coordinates
        const int nnodes = rModelPart.NumberOfNodes();
        ModelPart::NodesContainerType::iterator it_begin = rModelPart.NodesBegin();

        //#pragma omp parallel for
        for(int i = 0; i<nnodes; i++)
        {
          ModelPart::NodesContainerType::iterator it_node = it_begin + i;

          it_node->Coordinates()[0] = it_node->GetInitialPosition().x();
          it_node->Coordinates()[1] = it_node->GetInitialPosition().y();
          it_node->Coordinates()[2] = it_node->GetInitialPosition().z();
          it_node->Coordinates()[0] += it_node->FastGetSolutionStepValue(DISPLACEMENT)[0];
          it_node->Coordinates()[1] += it_node->FastGetSolutionStepValue(DISPLACEMENT)[1];
          it_node->Coordinates()[2] += it_node->FastGetSolutionStepValue(DISPLACEMENT)[2];
        }

      }

    }

    
  }

  /**
   * @brief Liberates internal storage for an element
   */
  virtual void Clear(Element::Pointer rCurrentElement)
  {
  }

  /**
   * @brief Liberates internal storage for a condition
   */
  virtual void Clear(Condition::Pointer rCurrentCondition)
  {
  }

  /**
   * @brief Liberates internal storage.
   */
  virtual void Clear() {}

  /**
   * @brief This function is designed to be called once to perform all the checks needed
   * @return 0 all ok
   */
  virtual int Check(ModelPart& rModelPart)
  {
    

    for(ModelPart::ElementsContainerType::iterator it=rModelPart.ElementsBegin();
        it!=rModelPart.ElementsEnd(); ++it)
    {
      it->Check(rModelPart.GetProcessInfo());
    }

    for(ModelPart::ConditionsContainerType::iterator it=rModelPart.ConditionsBegin();
        it!=rModelPart.ConditionsEnd(); ++it)
    {
      it->Check(rModelPart.GetProcessInfo());
    }

    for(typename IntegrationMethodsVectorType::iterator it=mTimeVectorIntegrationMethods.begin();
        it!=mTimeVectorIntegrationMethods.end(); ++it)
      (*it)->Check(rModelPart.GetProcessInfo());

    for(typename IntegrationMethodsScalarType::iterator it=mTimeScalarIntegrationMethods.begin();
        it!=mTimeScalarIntegrationMethods.end(); ++it)
      (*it)->Check(rModelPart.GetProcessInfo());

    

    return 0;
  }

  /** these functions is designed to be called in the builder and solver to introduce
      the selected time integration scheme. It "asks" the matrix needed to the element and
      performs the operations needed to introduce the seected time integration scheme.
      this function calculates at the same time the contribution to the LHS and to the RHS
      of the system
  */

  /**
   * This function is designed to be called in the builder and solver to introduce
   * @param pCurrentElement: The element to compute
   * @param rLHS_Contribution: The LHS matrix contribution
   * @param rRHS_Contribution: The RHS vector contribution
   * @param rEquationId: The ID's of the element degrees of freedom
   * @param rCurrentProcessInfo: The current process info instance
   */
  virtual void CalculateSystemContributions(Element::Pointer pCurrentElement,
                                            LocalSystemMatrixType& rLHS_Contribution,
                                            LocalSystemVectorType& rRHS_Contribution,
                                            Element::EquationIdVectorType& rEquationId,
                                            Process_Info& rCurrentProcessInfo)
  {
    pCurrentElement->CalculateLocalSystem(rLHS_Contribution, rRHS_Contribution, rCurrentProcessInfo);
    pCurrentElement->EquationIdVector(rEquationId, rCurrentProcessInfo);
  }

  /**
   * This function is designed to calculate just the RHS contribution
   * @param pCurrentElement: The element to compute
   * @param rRHS_Contribution: The RHS vector contribution
   * @param rEquationId: The ID's of the element degrees of freedom
   * @param rCurrentProcessInfo: The current process info instance
   */
  virtual void Calculate_RHS_Contribution(Element::Pointer pCurrentElement,
                                          LocalSystemVectorType& rRHS_Contribution,
                                          Element::EquationIdVectorType& rEquationId,
                                          Process_Info& rCurrentProcessInfo)
  {
    pCurrentElement->CalculateRightHandSide(rRHS_Contribution, rCurrentProcessInfo);
    pCurrentElement->EquationIdVector(rEquationId, rCurrentProcessInfo);
  }
  /**
   * This function is designed to calculate just the LHS contribution
   * @param pCurrentElement: The element to compute
   * @param rLHS_Contribution: The LHS matrix contribution
   * @param rEquationId: The ID's of the element degrees of freedom
   * @param rCurrentProcessInfo: The current process info instance
   */
  virtual void Calculate_LHS_Contribution(Element::Pointer pCurrentElement,
                                          LocalSystemMatrixType& rLHS_Contribution,
                                          Element::EquationIdVectorType& rEquationId,
                                          Process_Info& rCurrentProcessInfo)
  {
    std::cout<< " it is C_LHS "<<std::endl;
    pCurrentElement->CalculateLeftHandSide(rLHS_Contribution, rCurrentProcessInfo);
    pCurrentElement->EquationIdVector(rEquationId, rCurrentProcessInfo);
  }

  virtual void EquationId(Element::Pointer pCurrentElement,
                          Element::EquationIdVectorType& rEquationId,
                          Process_Info& rCurrentProcessInfo)
  {
    (pCurrentElement)->EquationIdVector(rEquationId, rCurrentProcessInfo);
  }

  /**
   * Functions totally analogous to the precedent but applied to the "condition" objects
   * @param pCurrentCondition: The condition to compute
   * @param rLHS_Contribution: The LHS matrix contribution
   * @param rRHS_Contribution: The RHS vector contribution
   * @param rEquationId: The ID's of the element degrees of freedom
   * @param rCurrentProcessInfo: The current process info instance
   */
  virtual void Condition_CalculateSystemContributions(Condition::Pointer pCurrentCondition,
                                                      LocalSystemMatrixType& rLHS_Contribution,
                                                      LocalSystemVectorType& rRHS_Contribution,
                                                      Element::EquationIdVectorType& rEquationId,
                                                      Process_Info& rCurrentProcessInfo)
  {
    pCurrentCondition->CalculateLocalSystem(rLHS_Contribution, rRHS_Contribution, rCurrentProcessInfo);
    pCurrentCondition->EquationIdVector(rEquationId, rCurrentProcessInfo);
  }
  /**
   * Functions totally analogous to the precedent but applied to the "condition" objects
   * @param rCurrentCondition: The condition to compute
   * @param rLHS_Contribution: The LHS matrix contribution
   * @param rRHS_Contribution: The RHS vector contribution
   * @param rEquationId: The ID's of the element degrees of freedom
   * @param rCurrentProcessInfo: The current process info instance
   */
  virtual void Condition_Calculate_RHS_Contribution(Condition::Pointer pCurrentCondition,
                                                    LocalSystemVectorType& rRHS_Contribution,
                                                    Element::EquationIdVectorType& rEquationId,
                                                    Process_Info& rCurrentProcessInfo)
  {
    pCurrentCondition->CalculateRightHandSide(rRHS_Contribution, rCurrentProcessInfo);
    pCurrentCondition->EquationIdVector(rEquationId, rCurrentProcessInfo);
  }

  /**
   * Functions that calculates the RHS of a "condition" object
   * @param rCurrentCondition: The condition to compute
   * @param rRHS_Contribution: The RHS vector contribution
   * @param rEquationId: The ID's of the condition degrees of freedom
   * @param rCurrentProcessInfo: The current process info instance
   */
  virtual void Condition_Calculate_LHS_Contribution(Condition::Pointer pCurrentCondition,
                                                    LocalSystemMatrixType& rLHS_Contribution,
                                                    Element::EquationIdVectorType& rEquationId,
                                                    Process_Info& rCurrentProcessInfo)
  {
    pCurrentCondition->CalculateLeftHandSide(rLHS_Contribution, rCurrentProcessInfo);
    pCurrentCondition->EquationIdVector(rEquationId, rCurrentProcessInfo);
  }

  virtual void Condition_EquationId(Condition::Pointer pCurrentCondition,
                                    Element::EquationIdVectorType& rEquationId,
                                    Process_Info& rCurrentProcessInfo)
  {
    (pCurrentCondition)->EquationIdVector(rEquationId, rCurrentProcessInfo);
  }

  /**
   * Function that returns the list of Degrees of freedom to be assembled in the system for a Given Element
   * @param rCurrentElement: The element to compute
   * @param rElementalDofsList: The element dofs list
   * @param rCurrentProcessInfo: The current process info instance
   */
  virtual void GetElementalDofList(Element::Pointer pCurrentElement,
                                   Element::DofsVectorType& rElementalDofList,
                                   Process_Info& rCurrentProcessInfo)
  {
    pCurrentElement->GetDofList(rElementalDofList, rCurrentProcessInfo);
  }

  /**
   * Function that returns the list of Degrees of freedom to be assembled in the system for a Given Element
   * @param rCurrentCondition: The condition to compute
   * @param rConditionDofsList: The condition dofs list
   * @param rCurrentProcessInfo: The current process info instance
   */
  virtual void GetConditionDofList(Condition::Pointer pCurrentCondition,
                                   Element::DofsVectorType& rConditionDofList,
                                   Process_Info& rCurrentProcessInfo)
  {
    pCurrentCondition->GetDofList(rConditionDofList, rCurrentProcessInfo);
  }

  ///@}
  ///@name Access
  ///@{


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
   * @brief Set process to execute after move_mesh
   */
  void SetProcess( ProcessPointerType pProcess )
  {
    mProcesses.push_back(pProcess); //NOTE: order set = order of execution
  }

  /**
   * @brief Set list of processes to execute after move_mesh
   */
  void SetProcessVector( ProcessPointerVectorType& rProcessVector )
  {
    mProcesses = rProcessVector;
  }

  ///@}
  ///@name Inquiry
  ///@{

  ///@}
  ///@name Friends
  ///@{

  ///@}
 protected:
  ///@name Protected static Member Variables
  ///@{

  ///@}
  ///@name Protected member Variables
  ///@{

  // Flags to set options
  Flags mOptions;

  // Time integration methods
  IntegrationMethodsVectorType mTimeVectorIntegrationMethods;
  IntegrationMethodsScalarType mTimeScalarIntegrationMethods;

  // Processes called after move mesh is called
  ProcessPointerVectorType mProcesses;

  ///@}
  ///@name Protected Operators
  ///@{

  ///@}
  ///@name Protected Operations
  ///@{

  /**
   * @brief Initialize the elements.
   * @details This is intended to be called just once when the strategy is initialized
   */
  virtual void InitializeElements(ModelPart& rModelPart)
  {
    

    if( this->IsNot(LocalFlagType::ELEMENTS_INITIALIZED) ){

//#pragma omp parallel for
      for(int i=0; i<static_cast<int>(rModelPart.Elements().size()); i++)
      {
        auto itElem = rModelPart.ElementsBegin() + i;
        itElem->Initialize(rModelPart.GetProcessInfo());
      }

    this->Set(LocalFlagType::ELEMENTS_INITIALIZED, true);

    }

    
  }

  /**
   * @brief Initialize the conditions.
   * @details This is intended to be called just once when the strategy is initialized
   */
  virtual void InitializeConditions(ModelPart& rModelPart)
  {
    

    if( this->IsNot(LocalFlagType::ELEMENTS_INITIALIZED) )
    {
      std::cerr<< "Before initilizing Conditions, initialize Elements FIRST" << std::endl;
      exit(0);
    }
    if( this->IsNot(LocalFlagType::CONDITIONS_INITIALIZED) ){

      //#pragma omp parallel for
      for(int i=0; i<static_cast<int>(rModelPart.Conditions().size()); i++)
      {
        auto itCond = rModelPart.ConditionsBegin() + i;
        itCond->Initialize(rModelPart.GetProcessInfo());
      }

      this->Set(LocalFlagType::CONDITIONS_INITIALIZED, true);
    }

    
  }

  /**
   * @brief Initialize the conditions.
   * @details this function must be called only once per iteration
   */
  virtual void InitializeNonLinearIteration(Condition::Pointer rCurrentCondition,
                                            Process_Info& rCurrentProcessInfo)
  {
    

    rCurrentCondition->InitializeNonLinearIteration(rCurrentProcessInfo);

    
  }

  /**
   * @brief Initialize the elements.
   * @details This is intended to be called every iteration
   */
  virtual void InitializeNonLinearIteration(Element::Pointer rCurrentElement,
                                            Process_Info& rCurrentProcessInfo)
  {
    rCurrentElement->InitializeNonLinearIteration(rCurrentProcessInfo);
  }


  virtual void IntegrationMethodUpdate(NodeType& rNode)
  {
    for(typename IntegrationMethodsVectorType::iterator it=mTimeVectorIntegrationMethods.begin();
        it!=mTimeVectorIntegrationMethods.end(); ++it)
      (*it)->Update(rNode);
    for(typename IntegrationMethodsScalarType::iterator it=mTimeScalarIntegrationMethods.begin();
        it!=mTimeScalarIntegrationMethods.end(); ++it)
      (*it)->Update(rNode);
  }

  virtual void IntegrationMethodPredict(NodeType& rNode)
  {
    for(typename IntegrationMethodsVectorType::iterator it=mTimeVectorIntegrationMethods.begin();
        it!=mTimeVectorIntegrationMethods.end(); ++it)
      (*it)->Predict(rNode);
    for(typename IntegrationMethodsScalarType::iterator it=mTimeScalarIntegrationMethods.begin();
        it!=mTimeScalarIntegrationMethods.end(); ++it)
      (*it)->Predict(rNode);
  }


  ///@}
  ///@name Protected  Access
  ///@{

  ///@}
  ///@name Protected Inquiry
  ///@{

  ///@}
  ///@name Protected LifeCycle
  ///@{

  ///@}
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
  ///@name Serialization
  ///@{

  ///@}
  ///@name Private Inquiry
  ///@{

  ///@}
  ///@name Un accessible methods
  ///@{

  ///@}
};

#endif // KRATOS_SOLUTION_SCHEME_H_INCLUDED defined
