#ifndef _VARIABLES_LIST_H_
#define _VARIABLES_LIST_H_
#include "variable.h"

#include <vector>
class Variables_List
{
    public:
        /// @name Type Define
        /// @{
            typedef std::size_t     
                                                                    SizeType;
            typedef std::size_t 
                                                                   IndexType;
            typedef double 
                                                                   BlockType;
            typedef std::vector<IndexType> 
                                                           KeysContainerType;
            typedef std::vector<IndexType> 
                                                      PositionsContainerType;
            typedef std::vector<const Variable_Data*> 
                                                      VariablesContainerType;
            typedef Variable_Data 
                                                                   data_type;
            typedef const Variable_Data* 
                                                                  value_type;
            typedef const Variable_Data* 
                                                               const_pointer;
            typedef Variable_Data const&
                                                             const_reference;
            typedef VariablesContainerType::size_type 
                                                                   size_type;
            typedef VariablesContainerType::const_iterator 
                                                          ptr_const_iterator;
            typedef VariablesContainerType::const_reverse_iterator 
                                                  ptr_const_reverse_iterator;
            typedef VariablesContainerType::difference_type 
                                                             difference_type;
            LOTUS_POINTER_DEFINE(Variables_List)
        /// @}


        /// @name Life Circle
        /// @{
            Variables_List()
            {

            }

            template<typename TInputIteratorType>
            Variables_List(TInputIteratorType First, TInputIteratorType Last)
            {
                for(;First!=Last;++First)
                    Add(*First);
            }

            Variables_List(const Variables_List& another)
            :mDataSize(another.mDataSize),
             mHashFunctionIndex(another.mHashFunctionIndex),
             mKeys(another.mKeys),
             mPositions(another.mPositions),
             mVariables(another.mVariables),
             mDofVariables(another.mDofVariables),
             mDofReactions(another.mDofReactions)
            {

            }

            ~Variables_List()
            {

            }
        /// @}


        /// @name Operators
        /// @{
            Variables_List& operator=(Variables_List const& rOther)
            {
                mDataSize = rOther.mDataSize;
                mHashFunctionIndex = rOther.mHashFunctionIndex;
                mKeys = rOther.mKeys;
                mPositions = rOther.mPositions;
                mVariables = rOther.mVariables;
                mDofVariables=rOther.mDofVariables;
                mDofReactions=rOther.mDofReactions;
                return *this;
            }

            IndexType operator()(IndexType VariableKey) const
            {
                return GetPosition(VariableKey);
            }
        /// @}


        /// @name Operations
        /// @{


        /// @}


        /// @name Access
        /// @{
            const Variable_Data& GetDofVariable(int DofIndex) const 
            {
                return *mDofVariables[DofIndex];
            }
        /// @}


        /// @name Inquiry
        /// @{
            IndexType Index(IndexType VariableKey) const
            {
                return GetPosition(VariableKey);
            }
            template<class TDataType>
            IndexType Index(Variable<TDataType> const& ThisVariable) const
            {
                return GetPosition(ThisVariable.SourceKey());
            }
            IndexType Index(const Variable_Data* pThisVariable) const
            {
                return GetPosition(pThisVariable->SourceKey());
            }
            SizeType DataSize() const
            {
                return mDataSize;
            }
            
            bool Has(const Variable_Data& rThisVariable) const
            {
                if(rThisVariable.IsComponent()){
                    return Has(rThisVariable.GetSourceVariable());
                }

                if (mPositions.empty())
                    return false;

                if (rThisVariable.SourceKey() == 0)
                    return false;

                return mKeys[GetHashIndex(rThisVariable.SourceKey(), mKeys.size(), mHashFunctionIndex)] == rThisVariable.SourceKey();
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
            SizeType mDataSize = 0;

            SizeType mHashFunctionIndex = 0;

            KeysContainerType mKeys = {static_cast<IndexType>(-1)};

            PositionsContainerType mPositions = {static_cast<IndexType>(-1)};

            VariablesContainerType mVariables;

            VariablesContainerType mDofVariables;

            VariablesContainerType mDofReactions;
        /// @}


        /// @name Private Operatiors
        /// @{


        /// @}


        /// @name Private Operations
        /// @{
            SizeType GetHashIndex(std::size_t Key, 
                                  std::size_t TableSize, 
                                  std::size_t HashFunctionIndex) const
            {
			    return (Key >> HashFunctionIndex) & (TableSize - 1);
            }

            SizeType GetPosition(IndexType Key) const 
            {
                SizeType index = GetHashIndex(Key,mPositions.size(),mHashFunctionIndex);
                return mPositions[index];
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