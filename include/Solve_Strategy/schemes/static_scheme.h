#ifndef _STATIC_SCHEME_H
#define _STATIC_SCHEME_H

// System includes

// External includes

// Project includes
#include "solution_scheme.h"

/** @brief Static integration scheme (for static problems)
 */
template<class TSparseSpace,  class TDenseSpace >
class Static_Scheme: public Solution_Scheme<TSparseSpace,TDenseSpace>
{
public:

  ///@name Type Definitions
  ///@{
  LOTUS_SHARED_POINTER_DEFINE( Static_Scheme );

  typedef Solution_Scheme<TSparseSpace,TDenseSpace>                            BaseType;
  typedef typename BaseType::SolutionSchemePointerType                  BasePointerType;

  typedef typename BaseType::NodeType                                          NodeType;
  typedef typename BaseType::DofsArrayType                                DofsArrayType;
  typedef typename Element::DofsVectorType                               DofsVectorType;
  typedef typename BaseType::SystemMatrixType                          SystemMatrixType;
  typedef typename BaseType::SystemVectorType                          SystemVectorType;
  typedef typename BaseType::LocalSystemVectorType                LocalSystemVectorType;
  typedef typename BaseType::LocalSystemMatrixType                LocalSystemMatrixType;

  typedef ModelPart::NodesContainerType                              NodesContainerType;
  typedef ModelPart::ElementsContainerType                        ElementsContainerType;
  typedef ModelPart::ConditionsContainerType                    ConditionsContainerType;

  typedef typename BaseType::IntegrationMethodsVectorType  IntegrationMethodsVectorType;
  typedef typename BaseType::IntegrationMethodsScalarType  IntegrationMethodsScalarType;

  ///@}
  ///@name Life Cycle
  ///@{

  /// Constructor.
  Static_Scheme(IntegrationMethodsVectorType& rTimeVectorIntegrationMethods, Flags& rOptions)
      :BaseType(rTimeVectorIntegrationMethods, rOptions)
  {
  }

  /// Constructor.
  Static_Scheme(IntegrationMethodsVectorType& rTimeVectorIntegrationMethods)
      :BaseType(rTimeVectorIntegrationMethods)
  {
  }


  /// Constructor.
  Static_Scheme(IntegrationMethodsScalarType& rTimeScalarIntegrationMethods, Flags& rOptions)
      :BaseType(rTimeScalarIntegrationMethods, rOptions)
  {
  }

  /// Constructor.
  Static_Scheme(IntegrationMethodsScalarType& rTimeScalarIntegrationMethods)
      :BaseType(rTimeScalarIntegrationMethods)
  {
  }

  /// Constructor.
  Static_Scheme(IntegrationMethodsVectorType& rTimeVectorIntegrationMethods,
                IntegrationMethodsScalarType& rTimeScalarIntegrationMethods,
                Flags& rOptions)
      :BaseType(rTimeVectorIntegrationMethods, rTimeScalarIntegrationMethods, rOptions)
  {
  }

  /// Constructor.
  Static_Scheme(IntegrationMethodsVectorType& rTimeVectorIntegrationMethods,
                IntegrationMethodsScalarType& rTimeScalarIntegrationMethods)
      :BaseType(rTimeVectorIntegrationMethods, rTimeScalarIntegrationMethods)
  {
  }

  /// Copy Constructor.
  Static_Scheme(Static_Scheme& rOther)
      :BaseType(rOther)
  {
  }

  /// Clone.
  BasePointerType Clone() override
  {
    return BasePointerType( new Static_Scheme(*this) );
  }

  /// Destructor.
  ~Static_Scheme() override {}

  ///@}
  ///@name Operators
  ///@{

  ///@}
  ///@name Operations
  ///@{

  /**
  this is the place to initialize the Scheme.
  This is intended to be called just once when the strategy is initialized
    */
  void Initialize(ModelPart& rModelPart) override
  {
    BaseType::Initialize(rModelPart);
  }

  /**
   * Performing the update of the solution
   * @param rModelPart: The model of the problem to solve
   * @param rDofSet: Set of all primary variables
   * @param rDx: incremental update of primary variables
   */

  void Update(ModelPart& rModelPart,
  DofsArrayType& rDofSet,
  SystemVectorType& rDx) override
  {
    this->UpdateDofs(rModelPart,rDofSet,rDx);
    this->UpdateVariables(rModelPart);
    this->MoveMesh(rModelPart);
  }

  /**
   * Performing the prediction of the solution
   * @param rModelPart: The model of the problem to solve
   * @param rDofSet set of all primary variables
   * @param rDx: Incremental update of primary variables
   */

  void Predict(ModelPart& rModelPart,
    DofsArrayType& rDofSet,
    SystemVectorType& rDx) override
  {
    this->PredictVariables(rModelPart);
    this->MoveMesh(rModelPart);
  }


  /**
   * This function is designed to be called once to perform all the checks needed
   * on the input provided. Checks can be "expensive" as the function is designed
   * to catch user's errors.
   * @param rModelPart: The model of the problem to solve
   * @return Zero means  all ok
   */

  int Check(ModelPart& rModelPart) override
  {
    // Perform base base checks
    int ErrorCode = 0;
    ErrorCode  = BaseType::Check(rModelPart);
    
    // Check for minimum value of the buffer index
    if (rModelPart.GetBufferSize() < 2)
      {
          std::cerr << "insufficient buffer size. Buffer size should be greater than 2. Current size is" << rModelPart.GetBufferSize() << std::endl;
          exit(0);
      }

    if ( this->mTimeVectorIntegrationMethods.size() == 0  && this->mTimeScalarIntegrationMethods.size() == 0 ) {
      std::cerr << "Time integration methods for Vector or Scalar variables NOT supplied" << std::endl;
      exit(0);
    }

    return ErrorCode;
  }

  ///@}
  ///@name Access
  ///@{

  ///@}
  ///@name Inquiry
  ///@{

  ///@}
  ///@name Input and output
  ///@{

  /// Turn back information as a string.
  std::string Info() const override
  {
      std::stringstream buffer;
      buffer << "Static_Scheme";
      return buffer.str();
  }

  /// Print information about this object.
  void PrintInfo(std::ostream& rOStream) const override
  {
      rOStream << "Static_Scheme";
  }

  /// Print object's data.
  void PrintData(std::ostream& rOStream) const override
  {
    rOStream << "Static_Scheme Data";
  }

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

  ///@}
  ///@name Protected Operators
  ///@{

  ///@}
  ///@name Protected Operations
  ///@{

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

#endif // KRATOS_STATIC_SCHEME_H_INCLUDED defined
