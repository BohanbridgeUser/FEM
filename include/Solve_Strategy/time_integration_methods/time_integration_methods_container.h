#ifndef _TIME_INTEGRATION_METHODS_CONTAINER_H_
#define _TIME_INTEGRATION_METHODS_CONTAINER_H_

// System includes

// External includes

// Project includes
#include "time_integration_method.h"

/** A container for time integration methods.
 * This class implements a container for setting all methods in ProcessInfo
 */
template<class TVariableType, class TValueType>
class Time_Integration_Methods_Container
{
public:
  ///@name Type Definitions
  ///@{

  /// Pointer definition of Time_Integration_Methods_Container
  LOTUS_SHARED_POINTER_DEFINE(Time_Integration_Methods_Container);

  typedef Time_Integration_Method<TVariableType,TValueType>                TimeIntegrationMethodType;
  typedef typename TimeIntegrationMethodType::Pointer             TimeIntegrationMethodPointerType;
  typedef std::string LabelType;

  ///@name Life Cycle
  ///@{

  /// Default constructor.
  Time_Integration_Methods_Container()
      :mTimeIntegrationMethods()
  {
  }

  /// Copy constructor.
  Time_Integration_Methods_Container(Time_Integration_Methods_Container const& rOther)
      :mTimeIntegrationMethods(rOther.mTimeIntegrationMethods)
  {
  }

  /// Destructor.
  virtual ~Time_Integration_Methods_Container() {}

  ///@}
  ///@name Operators
  ///@{
  ///@}
  ///@name Operations
  ///@{
  ///@}
  ///@name Access
  ///@{

  void Set(LabelType const& rLabel, TimeIntegrationMethodPointerType pTimeIntegrationMethod)
  {
    mTimeIntegrationMethods[rLabel] = pTimeIntegrationMethod;
  }

  TimeIntegrationMethodPointerType Get(LabelType const& rLabel)
  {
    return mTimeIntegrationMethods[rLabel];
  }

  bool Has(LabelType const& rLabel)
  {
    typename std::map<LabelType,TimeIntegrationMethodPointerType>::iterator it_method = mTimeIntegrationMethods.find(rLabel);

    if ( it_method != mTimeIntegrationMethods.end() )
      return true;
    else
      return false;
  }

  LabelType GetMethodVariableName(LabelType const& rLabel)
  {

    for(typename std::map<LabelType,TimeIntegrationMethodPointerType>::const_iterator it=mTimeIntegrationMethods.begin(); it!=mTimeIntegrationMethods.end(); ++it)
    {
      if( (*it->second).HasVariableName(rLabel) )
        return it->first;
    }

    return rLabel;
  }

  ///@}
  ///@name Inquiry
  ///@{
  ///@}
  ///@name Input and output
  ///@{

  /// Turn back information as a string.
  virtual std::string Info() const
  {
    std::stringstream buffer;
    buffer << "Time_Integration_Methods_Container\n";
    for(typename std::map<LabelType,TimeIntegrationMethodPointerType>::const_iterator it=mTimeIntegrationMethods.begin(); it!=mTimeIntegrationMethods.end(); ++it)
      buffer << "["<<it->first<<"] = "<<*(it->second)<< "\n";
    return buffer.str();
  }

  /// Print information about this object.
  virtual void PrintInfo(std::ostream& rOStream) const
  {
    rOStream << "Time_Integration_Methods_Container\n";
    for(typename std::map<LabelType,TimeIntegrationMethodPointerType>::const_iterator it=mTimeIntegrationMethods.begin(); it!=mTimeIntegrationMethods.end(); ++it)
      rOStream << "["<<it->first<<"] = "<<*(it->second)<<std::endl;

  }

  /// Print object's data.
  virtual void PrintData(std::ostream& rOStream) const
  {
    rOStream << "Time_Integration_Methods_Container Data";
  }

  ///@}
  ///@name Friends
  ///@{


  ///@}

protected:
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

  std::map<LabelType,TimeIntegrationMethodPointerType> mTimeIntegrationMethods;

  ///@}
  ///@name Private Operators
  ///@{
  ///@}
  ///@name Private Operations
  ///@{
  ///@}
  ///@name Serialization
  ///@{

  ///@}
  ///@}
  ///@name Private Inquiry
  ///@{

  ///@}
  ///@name Un accessible methods
  ///@{

  ///@}

public:

  template<class TVariable> 
  static bool HasProcessInfo(TVariable const& rV, Process_Info::Pointer& p) 
  { return p->Has(rV); }

  template<class TVariable> 
  static void AddToProcessInfo(TVariable const& rV,
                               typename TVariable::Type const& rValue, 
                               Process_Info::Pointer& p) 
  { p->SetValue(rV, rValue); }

  template<class TVariable> 
  static typename TVariable::Type GetFromProcessInfo(TVariable const& rV, Process_Info::Pointer& p) 
  { return p->GetValue(rV); }

}; // Class Time_Integration_Methods_Container
///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{
template<class TVariableType, class TValueType>
inline std::istream & operator >> (std::istream & rIStream, Time_Integration_Methods_Container<TVariableType,TValueType>& rThis)
{
  return rIStream;
}

template<class TVariableType, class TValueType>
inline std::ostream & operator << (std::ostream & rOStream, const Time_Integration_Methods_Container<TVariableType,TValueType>& rThis)
{
  return rOStream << rThis.Info();
}

#endif // KRATOS_TIME_INTEGRATION_METHODS_CONTAINER_H_INCLUDED defined
