#ifndef  _STATIC_METHOD_H_
#define  _STATIC_METHOD_H_

#include "time_integration_method.h"


/// Short class definition.
/** Detail class definition.
 * This class performs predict and update of dofs variables, their time derivatives and time integrals
 */
template<class TVariableType, class TValueType>
class Static_Method 
: public Time_Integration_Method<TVariableType,TValueType>
{
public:

  ///@name Type Definitions
  ///@{

  /// BaseType
  typedef Time_Integration_Method<TVariableType,TValueType>  BaseType;

  /// BasePointerType
  typedef typename BaseType::Pointer                  BasePointerType;

  /// NodeType
  typedef typename BaseType::NodeType                        NodeType;

  /// KratosVariable or KratosVariableComponent
  typedef typename BaseType::VariablePointer          VariablePointer;

  LOTUS_SHARED_POINTER_DEFINE( Static_Method );

  ///@}
  ///@name Life Cycle
  ///@{

    Static_Method()
    : BaseType() {}
    /// Constructor.
    Static_Method(const TVariableType& rVariable)
    :BaseType(rVariable) {}
    /// Constructor.
    Static_Method(const TVariableType& rVariable, const TVariableType& rFirstDerivative, const TVariableType& rSecondDerivative)
    :BaseType(rVariable,rFirstDerivative,rSecondDerivative) {}
    /// Constructor.
    Static_Method(const TVariableType& rVariable, const TVariableType& rFirstDerivative, const TVariableType& rSecondDerivative, const TVariableType& rPrimaryVariable)
    :BaseType(rVariable,rFirstDerivative,rSecondDerivative,rPrimaryVariable) {}
    /// Copy Constructor.
    Static_Method(Static_Method& rOther)
    :BaseType(rOther) {}
    /// Destructor.
    ~Static_Method() override{}

  ///@}
  ///@name Operators
  ///@{

  ///@}
  ///@name Operations
  ///@{
      BasePointerType Clone() override
      {
        return BasePointerType( new Static_Method(*this) );
      }
      void Assign(NodeType& rNode) override
      {

      }
      void Predict(NodeType& rNode) override
      {

      }
      void Update(NodeType& rNode) override
      {

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
      std::string Info() const override
      {
          std::stringstream buffer;
          buffer << "Static_Method";
          return buffer.str();
      }
      void PrintInfo(std::ostream& rOStream) const override
      {
          rOStream << "Static_Method";
      }
      void PrintData(std::ostream& rOStream) const override
      {
          rOStream << "Static_Method Data";
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

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

template<class TVariableType, class TValueType>
inline std::istream & operator >> (std::istream & rIStream, Static_Method<TVariableType,TValueType>& rThis)
{
  return rIStream;
}

template<class TVariableType, class TValueType>
inline std::ostream & operator << (std::ostream & rOStream, const Static_Method<TVariableType,TValueType>& rThis)
{
  return rOStream << rThis.Info();
}

#endif 
