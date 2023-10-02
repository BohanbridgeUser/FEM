#ifndef _TIME_INTEGRATION_METHOD_H_
#define _TIME_INTEGRATION_METHOD_H_

#include "../../process_info.h"
#include "../../Variable/variables.h"
#include "../../Node/node.h"
#include "../solve_local_flags.h"

/// Short class definition.
/** Detail class definition.
 * This class performs predict and update of dofs variables, their time derivatives and time integrals
 */
template<class TVariableType, class TValueType>
class Time_Integration_Method : public Flags
{
public:

  ///@name Type Definitions
  ///@{

  /// NodeType
  typedef Node NodeType;

  /// KratosVariable or KratosVariableComponent
  typedef const TVariableType*     VariablePointer;

  typedef const TValueType*           ValuePointer;

  typedef void (Time_Integration_Method::*MethodPointer) (NodeType& rNode);

  typedef double& (Time_Integration_Method::*MethodFactorPointer) (double& rParameter);

  LOTUS_SHARED_POINTER_DEFINE( Time_Integration_Method );

  typedef typename Time_Integration_Method::Pointer   Time_Integration_MethodPointer;

  ///@}
  ///@name Life Cycle
  ///@{


  /// Default Constructor.
  Time_Integration_Method() : Flags()
  {
    mpVariable = nullptr;
    mpFirstDerivative = nullptr;
    mpSecondDerivative = nullptr;

    mpPrimaryVariable = nullptr;
    mpInputVariable = nullptr;
  }

  /// Constructor.
  Time_Integration_Method(const TVariableType& rVariable) : Flags()
  {
    mpVariable = &rVariable;
    mpFirstDerivative = nullptr;
    mpSecondDerivative = nullptr;

    //default dof variable
    mpPrimaryVariable = &rVariable;

    this->SetPointerMethods();
  }

  /// Constructor.
  Time_Integration_Method(const TVariableType& rVariable, const TVariableType& rFirstDerivative, const TVariableType& rSecondDerivative) : Flags()
  {
    mpVariable = &rVariable;
    mpFirstDerivative = &rFirstDerivative;
    mpSecondDerivative = &rSecondDerivative;

    //default dof variable
    mpPrimaryVariable = &rVariable;

    this->SetPointerMethods();
  }

  /// Constructor.
  Time_Integration_Method(const TVariableType& rVariable, const TVariableType& rFirstDerivative, const TVariableType& rSecondDerivative, const TVariableType& rPrimaryVariable) : Flags()
  {
    mpVariable = &rVariable;
    mpFirstDerivative = &rFirstDerivative;
    mpSecondDerivative = &rSecondDerivative;

    if( HasVariableName(rPrimaryVariable.Name()) ){
      //default dof variable
      mpPrimaryVariable = &rPrimaryVariable;
    }
    else{
      std::cerr << "The primary variable supplied: "<<rPrimaryVariable.Name()<<" is not any of the time integration variables" << std::endl;
      exit(0);
    }

    this->SetPointerMethods();
  }

  /// Copy Constructor.
  Time_Integration_Method(Time_Integration_Method& rOther)
    :Flags(rOther)
    ,mpVariable(rOther.mpVariable)
    ,mpFirstDerivative(rOther.mpFirstDerivative)
    ,mpSecondDerivative(rOther.mpSecondDerivative)
    ,mpPrimaryVariable(rOther.mpPrimaryVariable)
    ,mpInputVariable(rOther.mpInputVariable)
  {
  }

  /// Clone
  virtual Time_Integration_MethodPointer Clone()
  {
    return std::make_shared<Time_Integration_Method>(*this);
  }

  /// Destructor.
  ~Time_Integration_Method() override{}

  ///@}
  ///@name Operators
  ///@{

  ///@}
  ///@name Operations
  ///@{

  // set parameters (to call it once with the original input parameters)
  virtual void CalculateParameters(Process_Info& rCurrentProcessInfo)
  {

  }

  // set parameters (do not calculate parameters here, only read them)
  virtual void SetParameters(const Process_Info& rCurrentProcessInfo)
  {

  }

  // set parameters to process info
  virtual void SetProcessInfoParameters(Process_Info& rCurrentProcessInfo)
  {

  }

  // set input variable (constrained or dof variable)
  void SetInputVariable(const TVariableType& rVariable)
  {
    mpInputVariable = &rVariable;

    this->SetPointerAssignMethod();
  }


  // get primary variable name
  std::string GetPrimaryVariableName()
  {
    return (*this->mpPrimaryVariable).Name();
  }

  // get primary variable name
  std::string GetVariableName()
  {
    return (*this->mpVariable).Name();
  }

  // check if the integration method has the variable
  bool HasVariableName(const std::string& rVariableName)
  {
    if( this->mpVariable != nullptr ){
      if( rVariableName == (*this->mpVariable).Name() ){
        return true;
      }
      else if( this->mpFirstDerivative != nullptr ){
        if( rVariableName == (*this->mpFirstDerivative).Name() ){
          return true;
        }
        else if( this->mpSecondDerivative != nullptr ){
          if( rVariableName == (*this->mpSecondDerivative).Name() ){
            return true;
          }
        }
      }
    }
    return false;
  }


  // check if the integration method has the step variable (step variable)
  virtual bool HasStepVariable()
  {
    return false;
  }

  // set step variable (step variable)
  virtual void SetStepVariable(const TVariableType& rStepVariable)
  {
    std::cerr << " Calling SetStepVariable from time integration base class " <<std::endl;
    exit(0);
  }

  // assign
  virtual void Assign(NodeType& rNode)
  {
    

    (this->*this->mpAssign)(rNode);

    
  }

  // predict
  virtual void Predict(NodeType& rNode)
  {
    

    (this->*this->mpPredict)(rNode);

    
  }

  // update
  virtual void Update(NodeType& rNode)
  {
    

    (this->*this->mpUpdate)(rNode);

    
  }

  /**
   * @brief This function is designed to be called once to perform all the checks needed
   * @return 0 all ok
   */
  virtual int Check( const Process_Info& rCurrentProcessInfo )
  {
    

    // Check that all required variables have been registered
    if( mpVariable == nullptr ){
      std::cerr << " time integration method Variable not set " <<std::endl;
      exit(0);
    }

    if( mpPrimaryVariable == nullptr ){
      std::cerr << " time integration method PrimaryVariable not set " <<std::endl;
      exit(0); 
    }

    return 0;

    
  }

  ///@}
  ///@name Access
  ///@{

  // get parameters for variables (RHS)
  virtual double& GetFirstDerivativeKineticFactor(double& rParameter)
  {
    
    return (this->*this->mpFirstDerivativeKineticFactor)(rParameter);
    
  }

  virtual double& GetSecondDerivativeKineticFactor(double& rParameter)
  {
    
    return (this->*this->mpSecondDerivativeKineticFactor)(rParameter);
    
  }


  // get parameters for matrices (LHS)
  virtual double& GetFirstDerivativeInertialFactor(double& rParameter)
  {
    
    return (this->*this->mpFirstDerivativeInertialFactor)(rParameter);
    
  }

  virtual double& GetSecondDerivativeInertialFactor(double& rParameter)
  {
    
    return (this->*this->mpSecondDerivativeInertialFactor)(rParameter);
    
  }

  ///@}
  ///@name Flags
  ///@{

  Flags& GetFlags()
  {
    return *this;
  }

  Flags const& GetFlags() const
  {
    return *this;
  }

  void SetFlags(Flags const& rThisFlags)
  {
    Flags::operator=(rThisFlags);
  }

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
    buffer << "Time_Integration_Method";
    return buffer.str();
  }

  /// Print information about this object.
  void PrintInfo(std::ostream& rOStream) const override
  {
    rOStream << "Time_Integration_Method";
  }

  /// Print object's data.
  void PrintData(std::ostream& rOStream) const override
  {
    rOStream << "Time_Integration_Method Data";
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


  // method variables and derivatives

  VariablePointer mpVariable;

  VariablePointer mpFirstDerivative;

  VariablePointer mpSecondDerivative;

  // primary variable (calculated variable 'dof')

  VariablePointer mpPrimaryVariable;


  // input variable (imposed variable or calculated variable)

  VariablePointer mpInputVariable;


  // method pointer

  MethodPointer mpAssign;

  MethodPointer mpPredict;

  MethodPointer mpUpdate;

  // dynamic integration method pointers

  MethodFactorPointer mpFirstDerivativeKineticFactor;
  MethodFactorPointer mpSecondDerivativeKineticFactor;

  MethodFactorPointer mpFirstDerivativeInertialFactor;
  MethodFactorPointer mpSecondDerivativeInertialFactor;

  ///@}
  ///@name Protected Operators
  ///@{

  ///@}
  ///@name Protected Operations
  ///@{

  // set methods from primary variable
  void SetPointerMethods()
  {
    if( this->mpPrimaryVariable != nullptr ){

      if( this->mpVariable != nullptr ){
        if( *this->mpPrimaryVariable == *this->mpVariable ){
          mpPredict = &Time_Integration_Method::PredictFromVariable;
          mpUpdate  = &Time_Integration_Method::UpdateFromVariable;

          mpFirstDerivativeKineticFactor = &Time_Integration_Method::GetFirstDerivativeKineticParameter;
          mpSecondDerivativeKineticFactor = &Time_Integration_Method::GetSecondDerivativeKineticParameter;

          mpFirstDerivativeInertialFactor = &Time_Integration_Method::GetFirstDerivativeInertialParameter;
          mpSecondDerivativeInertialFactor = &Time_Integration_Method::GetSecondDerivativeInertialParameter;
        }
        else if( this->mpFirstDerivative != nullptr ){
          if( *this->mpPrimaryVariable == *this->mpFirstDerivative ){
            mpPredict = &Time_Integration_Method::PredictFromFirstDerivative;
            mpUpdate  = &Time_Integration_Method::UpdateFromFirstDerivative;

            mpFirstDerivativeKineticFactor = &Time_Integration_Method::GetKineticParameter;
            mpSecondDerivativeKineticFactor = &Time_Integration_Method::GetFirstDerivativeKineticParameter;

            mpFirstDerivativeInertialFactor = &Time_Integration_Method::GetInertialParameter;
            mpSecondDerivativeInertialFactor = &Time_Integration_Method::GetFirstDerivativeInertialParameter;
          }
          else if( this->mpSecondDerivative != nullptr ){
            if( *this->mpPrimaryVariable == *this->mpSecondDerivative ){
              mpPredict = &Time_Integration_Method::PredictFromSecondDerivative;
              mpUpdate  = &Time_Integration_Method::UpdateFromSecondDerivative;

              mpFirstDerivativeKineticFactor = &Time_Integration_Method::GetKineticParameter;
              mpSecondDerivativeKineticFactor = &Time_Integration_Method::GetKineticParameter;

              mpFirstDerivativeInertialFactor = &Time_Integration_Method::GetInertialParameter;
              mpSecondDerivativeInertialFactor = &Time_Integration_Method::GetInertialParameter;
            }
          }
        }
      }
    }

  }

  // set methods from input variable
  void SetPointerAssignMethod()
  {
    if( this->mpInputVariable != nullptr ){

      if( this->mpVariable != nullptr ){
        if( *this->mpInputVariable == *this->mpVariable ){
          mpAssign  = &Time_Integration_Method::AssignFromVariable;
        }
        else if( this->mpFirstDerivative != nullptr ){
          if( *this->mpInputVariable == *this->mpFirstDerivative ){
            mpAssign  = &Time_Integration_Method::AssignFromFirstDerivative;
          }
          else if( this->mpSecondDerivative != nullptr ){
            if( *this->mpInputVariable == *this->mpSecondDerivative ){
              mpAssign  = &Time_Integration_Method::AssignFromSecondDerivative;
            }
          }
        }
      }
    }

  }

  virtual void AssignFromVariable(NodeType& rNode)
  {
    std::cerr << " Calling predict from variable from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void AssignFromFirstDerivative(NodeType& rNode)
  {
    std::cerr << " Calling predict from first derivative from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void AssignFromSecondDerivative(NodeType& rNode)
  {
    std::cerr << " Calling predict from second derivative from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void AssignVariable(NodeType& rNode)
  {
    std::cerr << " Calling predict variable from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void AssignFirstDerivative(NodeType& rNode)
  {
    std::cerr << " Calling predict first derivative from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void AssignSecondDerivative(NodeType& rNode)
  {
    std::cerr << " Calling predict second derivative from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void PredictFromVariable(NodeType& rNode)
  {
    std::cerr << " Calling predict from variable from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void PredictFromFirstDerivative(NodeType& rNode)
  {
    std::cerr << " Calling predict from first derivative from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void PredictFromSecondDerivative(NodeType& rNode)
  {
    std::cerr << " Calling predict from second derivative from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void PredictVariable(NodeType& rNode)
  {
    std::cerr << " Calling predict variable from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void PredictFirstDerivative(NodeType& rNode)
  {
    std::cerr << " Calling predict first derivative from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void PredictSecondDerivative(NodeType& rNode)
  {
    std::cerr << " Calling predict second derivative from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void UpdateFromVariable(NodeType& rNode)
  {
    std::cerr << " Calling update from variable from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void UpdateFromFirstDerivative(NodeType& rNode)
  {
    std::cerr << " Calling update from first derivative from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void UpdateFromSecondDerivative(NodeType& rNode)
  {
    std::cerr << " Calling update from second derivative from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void UpdateVariable(NodeType& rNode)
  {
    std::cerr << " Calling update variable from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void UpdateFirstDerivative(NodeType& rNode)
  {
    std::cerr << " Calling update first derivative from time integration base class " <<std::endl;
    exit(0);
  }

  virtual void UpdateSecondDerivative(NodeType& rNode)
  {
    std::cerr << " Calling update second derivative from time integration base class " <<std::endl;
    exit(0);
  }


  ///@}
  ///@name Protected  Access
  ///@{

  // get parameters for variables (RHS)
  virtual double& GetKineticParameter(double& rParameter)
  {
    rParameter = 0.0;
    return rParameter;
  }

  virtual double& GetFirstDerivativeKineticParameter(double& rParameter)
  {
    rParameter = 0.0;
    return rParameter;
  }

  virtual double& GetSecondDerivativeKineticParameter(double& rParameter)
  {
    rParameter = 0.0;
    return rParameter;
  }


  // get parameters for matrices (LHS)
  virtual double& GetInertialParameter(double& rParameter)
  {
    rParameter = 1.0;
    return rParameter;
  }

  virtual double& GetFirstDerivativeInertialParameter(double& rParameter)
  {
    rParameter = 1.0;
    return rParameter;
  }

  virtual double& GetSecondDerivativeInertialParameter(double& rParameter)
  {
    rParameter = 1.0;
    return rParameter;
  }

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
  ///@name Private Inquiry
  ///@{

  ///@}
  ///@name Un accessible methods
  ///@{

  ///@}
}; // Class Time_Integration_Method

///@}

///@name Type Definitions
///@{

///@}
///@name Input and output
///@{

template<class TVariableType, class TValueType>
inline std::istream & operator >> (std::istream & rIStream, Time_Integration_Method<TVariableType,TValueType>& rThis)
{
  return rIStream;
}

template<class TVariableType, class TValueType>
inline std::ostream & operator << (std::ostream & rOStream, const Time_Integration_Method<TVariableType,TValueType>& rThis)
{
  return rOStream << rThis.Info();
}

#endif
