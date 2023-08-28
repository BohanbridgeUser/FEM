#ifndef _DATA_VALUE_CONTAINER_H_
#define _DATA_VALUE_CONTAINER_H_

// System includes
#include <string>
#include <iostream>
#include <cstddef>
#include <vector>


// External includes


// Project includes
#include "../define.h"
#include "../Variable/variable.h"
#include "flags.h"
class Data_Value_Container
{

public:
    ///@name Type Definitions
    ///@{
    LOTUS_DEFINE_LOCAL_FLAGS(OVERWRITE_OLD_VALUES);

    /// Pointer definition of DataValueContainer
    LOTUS_POINTER_DEFINE(Data_Value_Container);

    /// Type of the container used for variables
    typedef std::pair<const Variable_Data*, void*> ValueType;

    /// Type of the container used for variables
    typedef std::vector<ValueType> ContainerType;

    /// Type of the container used for variables
    typedef std::vector<ValueType>::iterator iterator;

    /// Type of the container used for variables
    typedef std::vector<ValueType>::const_iterator const_iterator;

    /// Type of the container used for variables
    typedef std::vector<ValueType>::size_type SizeType;

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    Data_Value_Container() {}

    /// Copy constructor.
    Data_Value_Container(Data_Value_Container const& rOther)
    {
        for(const_iterator i = rOther.mData.begin() ; i != rOther.mData.end() ; ++i)
            mData.push_back(ValueType(i->first, i->first->Clone(i->second)));
    }

    /// Destructor.
    virtual ~Data_Value_Container()
    {
        for(iterator i = mData.begin() ; i != mData.end() ; ++i)
            i->first->Delete(i->second);
    }


    ///@}
    ///@name Operators
    ///@{

    template<class TDataType> const TDataType& operator()(const Variable_Data& rThisVariable) const
    {
        return GetValue<TDataType>(rThisVariable);
    }

    template<class TDataType> TDataType& operator()(const Variable<TDataType>& rThisVariable)
    {
        return GetValue<TDataType>(rThisVariable);
    }

    template<class TDataType> const TDataType& operator()(const Variable<TDataType>& rThisVariable) const
    {
        return GetValue<TDataType>(rThisVariable);
    }

    template<class TDataType> TDataType& operator[](const Variable_Data& rThisVariable)
    {
        return GetValue<TDataType>(rThisVariable);
    }

    template<class TDataType> const TDataType& operator[](const Variable_Data& rThisVariable) const
    {
        return GetValue<TDataType>(rThisVariable);
    }

    template<class TDataType> TDataType& operator[](const Variable<TDataType>& rThisVariable)
    {
        return GetValue<TDataType>(rThisVariable);
    }

    template<class TDataType> const TDataType& operator[](const Variable<TDataType>& rThisVariable) const
    {
        return GetValue<TDataType>(rThisVariable);
    }

    iterator begin()
    {
        return mData.begin();
    }

    const_iterator begin() const
    {
        return mData.begin();
    }

    iterator end()
    {
        return mData.end();
    }

    const_iterator end() const
    {
        return mData.end();
    }

    /// Assignment operator.
    Data_Value_Container& operator=(const Data_Value_Container& rOther)
    {
        Clear();

        for(const_iterator i = rOther.mData.begin() ; i != rOther.mData.end() ; ++i)
            mData.push_back(ValueType(i->first, i->first->Clone(i->second)));

        return *this;
    }

    ///@}
    ///@name Operations
    ///@{

    template<class TDataType> 
    TDataType& GetValue(const Variable<TDataType>& rThisVariable)
    {
        typename ContainerType::iterator i;

        if ((i = std::find_if(mData.begin(), mData.end(), IndexCheck(rThisVariable.SourceKey())))  != mData.end())
            return *(static_cast<TDataType*>(i->second) + rThisVariable.GetComponentIndex());

        auto p_source_variable = &rThisVariable.GetSourceVariable();
        mData.push_back(ValueType(p_source_variable,p_source_variable->Clone(p_source_variable->pZero())));

        return *(static_cast<TDataType*>(mData.back().second) + rThisVariable.GetComponentIndex());
    }

    //TODO: make the variable of the constant version consistent with the one of the "classical" one
    template<class TDataType> 
    const TDataType& GetValue(const Variable<TDataType>& rThisVariable) const
    {
        typename ContainerType::const_iterator i;

        if ((i = std::find_if(mData.begin(), mData.end(), IndexCheck(rThisVariable.SourceKey())))  != mData.end())
            return *(static_cast<const TDataType*>(i->second) + rThisVariable.GetComponentIndex());

        return rThisVariable.Zero();
    }

    SizeType Size()
    {
        return mData.size();
    }

    template<class TDataType> 
    void SetValue(const Variable<TDataType>& rThisVariable, TDataType const& rValue)
    {
        typename ContainerType::iterator i;

        if ((i = std::find_if(mData.begin(), mData.end(), IndexCheck(rThisVariable.SourceKey())))  != mData.end()) {
            *(static_cast<TDataType*>(i->second) + rThisVariable.GetComponentIndex()) = rValue;
        } else {
            auto p_source_variable = &rThisVariable.GetSourceVariable();
            mData.push_back(ValueType(p_source_variable,p_source_variable->Clone(p_source_variable->pZero())));
            *(static_cast<TDataType*>(mData.back().second) + rThisVariable.GetComponentIndex()) = rValue;
        }
    }

    template<class TDataType> 
    void Erase(const Variable<TDataType>& rThisVariable)
    {
        typename ContainerType::iterator i;

        if ((i = std::find_if(mData.begin(), mData.end(), IndexCheck(rThisVariable.SourceKey())))  != mData.end())
        {
            i->first->Delete(i->second);
            mData.erase(i);
        }
    }

    void Clear()
    {
        for(ContainerType::iterator i = mData.begin() ; i != mData.end() ; i++)
            i->first->Delete(i->second);

        mData.clear();
    }

    void Merge(const Data_Value_Container& rOther, const Flags Options);

    ///@}
    ///@name Access
    ///@{


    ///@}
    ///@name Inquiry
    ///@{

    template<class TDataType> 
    bool Has(const Variable<TDataType>& rThisVariable) const
    {
        return (std::find_if(mData.begin(), mData.end(), IndexCheck(rThisVariable.SourceKey())) != mData.end());
    }

    bool IsEmpty() const
    {
        return mData.empty();
    }

    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    virtual std::string Info() const
    {
        return std::string("data value container");
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << "data value container";
    }

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const
    {
        for(const_iterator i = mData.begin() ; i != mData.end() ; ++i)
        {
            rOStream <<"    ";
            i->first->Print(i->second, rOStream);
            rOStream << std::endl;
        }
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

    class IndexCheck
    {
        std::size_t mI;
    public:
        explicit IndexCheck(std::size_t I) : mI(I) {}
        bool operator()(const ValueType& I)
        {
            return I.first->SourceKey() == mI;
        }
    };

    ///@name Static Member Variables
    ///@{


    ///@}
    ///@name Member Variables
    ///@{

    ContainerType mData;

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
    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Un accessible methods
    ///@{


    ///@}

}; // Class DataValueContainer


///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{


/// input stream function
inline std::istream& operator >> (std::istream& rIStream,
                                  Data_Value_Container& rThis);

/// output stream function
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Data_Value_Container& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}

#endif // KRATOS_DATA_VALUE_CONTAINER_H_INCLUDED  defined


