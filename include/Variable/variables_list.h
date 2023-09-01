#ifndef _VARIABLES_LIST_H_
#define _VARIABLES_LIST_H_
#include "variable.h"

#include <vector>
#include <cassert>
#include <boost/iterator/indirect_iterator.hpp>
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
            typedef boost::indirect_iterator<VariablesContainerType::const_iterator>
                                                              const_iterator;
		    typedef boost::indirect_iterator<VariablesContainerType::const_reverse_iterator>
                                                      const_reverse_iterator;
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
            template<class TDataType>
            IndexType operator()(Variable<TDataType> const& ThisVariable) const
            {
                return GetPosition(ThisVariable.SourceKey());
            }
            const Variable_Data* operator[](IndexType Index) const
            {
                return mVariables[Index];
            }
        /// @}


        /// @name Operations
        /// @{
            const_iterator             begin() const
            {
                return const_iterator(mVariables.begin());
            }
            const_iterator             end() const
            {
                return const_iterator(mVariables.end());
            }
            const_reverse_iterator     rbegin() const
            {
                return const_reverse_iterator(mVariables.rbegin());
            }
            const_reverse_iterator     rend() const
            {
                return const_reverse_iterator(mVariables.rend());
            }
            ptr_const_iterator         ptr_begin() const
            {
                return mVariables.begin();
            }
            ptr_const_iterator         ptr_end() const
            {
                return mVariables.end();
            }
            ptr_const_reverse_iterator ptr_rbegin() const
            {
                return mVariables.rbegin();
            }
            ptr_const_reverse_iterator ptr_rend() const
            {
                return mVariables.rend();
            }
            const_reference  front() const /* nothrow */
            {
                assert(!IsEmpty());
                return *mVariables.front();
            }
            const_reference  back() const  /* nothrow */
            {
                assert(!IsEmpty());
                return *mVariables.back();
            }
            size_type size() const
            {
                return mVariables.size();
            }
            size_type max_size() const
            {
                return mVariables.max_size();
            }

            void swap(Variables_List& rOther)
            {
                SizeType temp = mDataSize;
                mDataSize = rOther.mDataSize;
                rOther.mDataSize = temp;

                temp = mHashFunctionIndex;
                mHashFunctionIndex = rOther.mHashFunctionIndex;
                rOther.mHashFunctionIndex = temp;

                mVariables.swap(rOther.mVariables);

                mDofVariables.swap(rOther.mDofVariables);
                mDofReactions.swap(rOther.mDofReactions);

                mKeys.swap(rOther.mKeys);
                mPositions.swap(rOther.mPositions);
            }
            template<class TOtherDataType>
            void push_back(TOtherDataType const& x)
            {
                Add(x);
            }
            void clear()
            {
                mDataSize = 0;
                mHashFunctionIndex = 0;
                mVariables.clear();
                mDofVariables.clear();
                mDofReactions.clear();
                mKeys = {static_cast<IndexType>(-1)};
                mPositions = {static_cast<IndexType>(-1)};
            }
            void Add(Variable_Data const& ThisVariable)
            {
                if (ThisVariable.SourceKey() == 0)
                    std::cerr << "Adding uninitialize variable to this variable list. Check if all variables are registered before kernel initialization\n";

                if (Has(ThisVariable))
                    return;
                if(ThisVariable.IsComponent()){
                    Add(ThisVariable.GetSourceVariable());
                    return;
                }

                mVariables.push_back(&ThisVariable);
                SetPosition(ThisVariable.SourceKey(), mDataSize);
                const SizeType block_size = sizeof(BlockType);
                mDataSize += static_cast<SizeType>(((block_size - 1) + ThisVariable.Size()) / block_size);
            }

            int AddDof(Variable_Data const* pThisDofVariable){

                for(std::size_t dof_index = 0 ; dof_index < mDofVariables.size() ; dof_index++){
                    if(*mDofVariables[dof_index] == *pThisDofVariable){
                        return static_cast<int>(dof_index);
                    }
                }
                mDofVariables.push_back(pThisDofVariable);
                mDofReactions.push_back(nullptr);
                if(mDofVariables.size()>64) {
                    std::cout<< "Adding too many dofs to the node. Each node only can store 64 Dofs." << std::endl;
                    exit(0);
                }
                return mDofVariables.size() - 1;
            }

            int AddDof(Variable_Data const* pThisDofVariable, Variable_Data const* pThisDofReaction){

                for(std::size_t dof_index = 0 ; dof_index < mDofVariables.size() ; dof_index++){
                    if(*mDofVariables[dof_index] == *pThisDofVariable){
                        mDofReactions[dof_index] = pThisDofReaction;
                        return static_cast<int>(dof_index);
                    }
                }
                mDofVariables.push_back(pThisDofVariable);
                mDofReactions.push_back(pThisDofReaction);

                if(mDofVariables.size()>64) {
                    std::cerr << "Adding too many dofs to the node. Each node only can store 64 Dofs." << std::endl;
                    exit(0);
                }
                return mDofVariables.size() - 1;
            }
        /// @}


        /// @name Access
        /// @{
            const Variable_Data& GetDofVariable(int DofIndex) const 
            {
                return *mDofVariables[DofIndex];
            }
            const Variable_Data* pGetDofReaction(int DofIndex) const 
            {
			    return mDofReactions[DofIndex];
		    }
            void SetDofReaction(Variable_Data const* pThisDofReaction, int DofIndex) 
            {
                if(static_cast<std::size_t>(DofIndex) >= mDofReactions.size()) 
                {
                    std::cerr << "The given dof with index = " << DofIndex  << " is not stored in this variables list" << std::endl;
                    exit(0);
                }
                mDofReactions[DofIndex] = pThisDofReaction;
            }
            VariablesContainerType const& Variables()
            {
                return mVariables;
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
            bool IsEmpty() const
            {
                return mVariables.empty();
            }
        /// @}

        /// @name Input and Output
        /// @{
            std::string Info() const
            {
                return "variables list";
            }
            void PrintInfo(std::ostream& rOStream) const
            {
                rOStream << Info();
            }
            void PrintData(std::ostream& rOStream) const
            {
                rOStream << " with " << size() << " variables";
                rOStream << " (size : " << mDataSize << " blocks of " << sizeof(BlockType) << " bytes) " << std::endl;
                for (IndexType i = 0; i < mVariables.size(); ++i)
                    rOStream << "    " << mVariables[i]->Name() << " \t-> " << GetPosition(mVariables[i]->Key()) << std::endl;

                rOStream << " with " << mDofVariables.size() << " Dofs:";
                for (IndexType i = 0; i < mDofVariables.size(); ++i)
                    rOStream << "    [" << mDofVariables[i]->Name() << " ,  " << ((mDofReactions[i] == nullptr) ? "NONE" : mDofReactions[i]->Name()) << "]" << std::endl;
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
            /**
             * @brief @param mDataSize is the number of blocks
             * In Variable_List Class block is 8Bytes.
            */
            SizeType mDataSize = 0;

            SizeType mHashFunctionIndex = 0;

            /**
             * @brief Key is key of Variable
            */
            KeysContainerType mKeys = {static_cast<IndexType>(-1)};

            /**
             * @param Position is data of Variable in Variable_List_Data_Value_Container
            */
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

            void SetPosition(IndexType Key, SizeType ThePosition) 
            {
                if (mPositions.empty())
                    ResizePositions();

                if (mPositions[GetHashIndex(Key,mPositions.size(),mHashFunctionIndex)] < mDataSize) // The position is ocupied and a resize  (as re hash) is needed
                    ResizePositions();

                mKeys[GetHashIndex(Key, mPositions.size(), mHashFunctionIndex)] = Key;
                mPositions[GetHashIndex(Key, mPositions.size(), mHashFunctionIndex)] = ThePosition;
            }

            void ResizePositions() 
            {
                bool size_is_ok = false;
                std::size_t new_size = mPositions.size();
                SizeType new_hash_function_index = 0;
                while (size_is_ok != true) {
                    new_hash_function_index++;
                    if (new_hash_function_index > 31) {
                        new_hash_function_index = 0;
                        new_size *= 2;
                    }
                    KeysContainerType new_keys(new_size, static_cast<IndexType>(-1));
                    PositionsContainerType new_positions(new_size, static_cast<IndexType>(-1));
                    size_is_ok = true;

                        for (auto i_variable = mVariables.begin(); i_variable != mVariables.end(); i_variable++)
                            if (new_positions[GetHashIndex((*i_variable)->SourceKey(), new_size, new_hash_function_index)] > mDataSize) {
                                new_positions[GetHashIndex((*i_variable)->SourceKey(), new_size, new_hash_function_index)] = mPositions[GetHashIndex((*i_variable)->SourceKey(), mPositions.size(), mHashFunctionIndex)];
                                new_keys[GetHashIndex((*i_variable)->SourceKey(), new_size, new_hash_function_index)] = (*i_variable)->SourceKey();
                            }
                            else {
                                size_is_ok = false;
                                break;
                            }

                    if (size_is_ok) {
                        mPositions.swap(new_positions);
                        mKeys.swap(new_keys);
                        mHashFunctionIndex = new_hash_function_index;
                    }
                }
            }
        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};
inline std::ostream& operator << (std::ostream& rOStream,
		const Variables_List& rThis)
{
    rThis.PrintInfo(rOStream);
    rThis.PrintData(rOStream);

    return rOStream;
}
#endif