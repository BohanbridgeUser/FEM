#ifndef _VARIABLES_LIST_DATA_VALUE_CONTAINER_H_
#define _VARIABLES_LIST_DATA_VALUE_CONTAINER_H_

#include "variable.h"
#include "variables_list.h"
#include <cstdlib>
#include <cstring>

class Variables_List_Data_Value_Container
{
    public:
        /// @name Type Define
        /// @{
            using BlockType = Variables_List::BlockType;
            using ContainerType= BlockType*;
            using IndexType = std::size_t;
            using SizeType = std::size_t;

        /// @}


        /// @name Life Circle
        /// @{
            explicit Variables_List_Data_Value_Container(SizeType NewQueueSize = 1)
            : mQueueSize(NewQueueSize), mpCurrentPosition(0),
            mpData(0), mpVariablesList(nullptr)
            {
                if(!mpVariablesList)
                    return;
                Allocate();
                // Setting the current position at the begining of data
                mpCurrentPosition = mpData;

                const SizeType size = mpVariablesList->DataSize();
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    BlockType* position = Position(*it_variable);
                    for(SizeType i = 0 ; i < mQueueSize ; i++) {
                        it_variable->AssignZero(position + i * size);
                    }
                }
            }
            Variables_List_Data_Value_Container(Variables_List_Data_Value_Container const& rOther)
            : mQueueSize(rOther.mQueueSize), mpCurrentPosition(0),
            mpData(0), mpVariablesList(rOther.mpVariablesList)
            {
                if(!mpVariablesList)
                    return;
                Allocate();
                // Setting the current position with relative source container offset
                mpCurrentPosition = mpData + (rOther.mpCurrentPosition - rOther.mpData);

                const SizeType size = mpVariablesList->DataSize();
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    const SizeType offset = LocalOffset(*it_variable);
                    for(SizeType i = 0 ; i < mQueueSize ; i++) {
                        const SizeType total_offset =  offset + i * size;
                        it_variable->Copy(rOther.mpData + total_offset, mpData + total_offset);
                    }
                }
            }
            Variables_List_Data_Value_Container(Variables_List::Pointer  pVariablesList, SizeType NewQueueSize = 1)
            : mQueueSize(NewQueueSize), mpCurrentPosition(0),
            mpData(0), mpVariablesList(pVariablesList)
            {
                if(!mpVariablesList)
                    return;
                Allocate();
                // Setting the current position at the begining of data
                mpCurrentPosition = mpData;

                const SizeType size = mpVariablesList->DataSize();
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    BlockType*  position = Position(*it_variable);
                    for(SizeType i = 0 ; i < mQueueSize ; i++) {
                        it_variable->AssignZero(position + i * size);
                    }
                }
            }
            Variables_List_Data_Value_Container(Variables_List::Pointer  pVariablesList, BlockType const * ThisData, SizeType NewQueueSize = 1)
            : mQueueSize(NewQueueSize), mpCurrentPosition(0),
            mpData(0), mpVariablesList(pVariablesList)
            {
                if(!mpVariablesList)
                    return;
                Allocate();

                // Setting the current position at the begining of data
                mpCurrentPosition = mpData;

                const SizeType size = mpVariablesList->DataSize();
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    const SizeType offset = LocalOffset(*it_variable);
                    for(SizeType i = 0 ; i < mQueueSize ; i++) {
                        const SizeType total_offset =  offset + i * size;
                        it_variable->Copy(ThisData + total_offset, mpData + total_offset);
                    }
                }
            }

            virtual ~Variables_List_Data_Value_Container()
            {
                Clear();
            }
        /// @}


        /// @name Operators
        /// @{
            template<class TDataType>
            TDataType& operator()(const Variable<TDataType>& rThisVariable)
            {
                return GetValue(rThisVariable);
            }
            template<class TDataType>
            TDataType& operator()(const Variable<TDataType>& rThisVariable, SizeType QueueIndex)
            {
                return GetValue(rThisVariable, QueueIndex);
            }
            template<class TDataType>
            const TDataType& operator()(const Variable<TDataType>& rThisVariable) const
            {
                return GetValue(rThisVariable);
            }
            template<class TDataType>
            const TDataType& operator()(const Variable<TDataType>& rThisVariable, SizeType QueueIndex) const
            {
                return GetValue(rThisVariable, QueueIndex);
            }

            template<class TDataType> 
            TDataType& operator[](const Variable<TDataType>& rThisVariable)
            {
                return GetValue(rThisVariable);
            }
            template<class TDataType> 
            const TDataType& operator[](const Variable<TDataType>& rThisVariable) const
            {
                return GetValue(rThisVariable);
            }

            Variables_List_Data_Value_Container& operator=(const Variables_List_Data_Value_Container& rOther)
            {
                if(rOther.mpVariablesList == 0) 
                {
                    Clear();
                } 
                else if((mpVariablesList == rOther.mpVariablesList) && (mQueueSize == rOther.mQueueSize)) 
                {
                    mpCurrentPosition = mpData + (rOther.mpCurrentPosition - rOther.mpData);
                    const SizeType size = mpVariablesList->DataSize();
                    for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) 
                    {
                        const SizeType offset = LocalOffset(*it_variable);
                        for(SizeType i = 0 ; i < mQueueSize ; i++) 
                        {
                            const SizeType total_offset =  offset + i * size;
                            it_variable->Assign(rOther.mpData + total_offset, mpData + total_offset);
                        }
                    }
                } 
                else 
                {
                    DestructAllElements();
                    mQueueSize = rOther.mQueueSize;
                    mpVariablesList = rOther.mpVariablesList;
                    Reallocate();
                    mpCurrentPosition = mpData + (rOther.mpCurrentPosition - rOther.mpData);
                    const SizeType size = mpVariablesList->DataSize();
                    for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) 
                    {
                        const SizeType offset = LocalOffset(*it_variable);
                        for(SizeType i = 0 ; i < mQueueSize ; i++) 
                        {
                            const SizeType total_offset =  offset + i * size;
                            it_variable->Copy(rOther.mpData + total_offset, mpData + total_offset);
                        }
                    }
                }
                return *this;
            }
        /// @}


        /// @name Operations
        /// @{
            void Resize(SizeType NewSize)
            {
                if(mQueueSize == NewSize) 
                    return; 
                if(!mpVariablesList)
                    return;
                if(mQueueSize > NewSize) 
                { 
                    for(SizeType i = NewSize ; i < mQueueSize ; i++)
                        DestructElements(i);
                    const SizeType size = mpVariablesList->DataSize();
                    //allocating memory
                    BlockType* temp = (BlockType*)malloc(size * sizeof(BlockType) * NewSize);
                    //Copying data to allocated memory
                    for(SizeType i = 0 ; i < NewSize ; i++)
                        memcpy(temp + i * size, Position(i), size * sizeof(BlockType));
                    // Updating the queue size
                    mQueueSize = NewSize;
                    // freeing the old memory
                    free(mpData);
                    mpData = temp;
                    mpCurrentPosition = mpData;
                } else {
                    // Calculating the difference of new and old sizes
                    const SizeType difference = NewSize - mQueueSize;
                    //keeping the old queue size
                    const SizeType old_size = mQueueSize;
                    // Getting the relative offset of current position
                    const SizeType current_offset = mpCurrentPosition - mpData;
                    //Updating the queue size
                    mQueueSize = NewSize;
                    // Reallocating for new size
                    Reallocate();
                    SizeType size = mpVariablesList->DataSize();
                    // Updating the mpCurrentPosition
                    mpCurrentPosition = mpData + current_offset;
                    // moving the region after current position to the end
                    const SizeType region_size = old_size * size - current_offset;
                    memmove(mpCurrentPosition + difference * size, mpCurrentPosition, region_size * sizeof(BlockType));
                    // Assigning zero to the added elements
                    for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                        BlockType*  position = mpCurrentPosition + LocalOffset(*it_variable);
                        for(SizeType i = 0 ; i < difference ; i++) {
                            it_variable->AssignZero(position + i * size);
                        }
                    }
                    //moving the current position to the moved place
                    mpCurrentPosition +=  difference * size;
                }
            }
            void SetVariablesList(Variables_List::Pointer pVariablesList)
            {
                DestructAllElements();

                mpVariablesList = pVariablesList;

                if(!mpVariablesList)
                    return;

                Reallocate();

                mpCurrentPosition = mpData;

                const SizeType size = mpVariablesList->DataSize();
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    BlockType*  position = Position(*it_variable);
                    for(SizeType i = 0 ; i < mQueueSize ; i++) {
                        it_variable->AssignZero(position + i * size);
                    }
                }
            }
            void SetVariablesList(Variables_List::Pointer pVariablesList, SizeType ThisQueueSize)
            {
                DestructAllElements();

                mpVariablesList = pVariablesList;

                mQueueSize = ThisQueueSize;

                if(!mpVariablesList)
                    return;

                Reallocate();

                mpCurrentPosition = mpData;

                const SizeType size = mpVariablesList->DataSize();
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    BlockType*  position = Position(*it_variable);
                    for(SizeType i = 0 ; i < mQueueSize ; i++) {
                        it_variable->AssignZero(position + i * size);
                    }
                }
            }
            void AssignData(BlockType* Source, SizeType QueueIndex)
            {
                AssignData(Source, Position(QueueIndex));
            }

            void CloneFront()
            {
                if(mQueueSize == 0) 
                {
                    Resize(1);
                    return;
                }
                if(mQueueSize == 1)
                    return;
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                const SizeType size = mpVariablesList->DataSize();
                BlockType* position = (mpCurrentPosition == mpData) ? mpData + TotalSize() - size :  mpCurrentPosition - size;
                AssignData(mpCurrentPosition, position);
                mpCurrentPosition = position;
            }
            void PushFront()
            {
                if(mQueueSize == 0) {
                    Resize(1);
                    return;
                }
                if(mQueueSize == 1)
                    return;
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                const SizeType size = mpVariablesList->DataSize();
                mpCurrentPosition = (mpCurrentPosition == mpData) ? mpData + TotalSize() - size :  mpCurrentPosition - size;
                AssignZero();

            }
            void AssignZero()
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    it_variable->AssignZero(mpCurrentPosition + LocalOffset(*it_variable)); 
                }
            }
            void AssignZero(const SizeType QueueIndex)
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                BlockType* position = Position(QueueIndex);
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    it_variable->AssignZero(position + LocalOffset(*it_variable));
                }
            }

            void Clear()
            {
                DestructAllElements();
                if(mpData)
                    free(mpData);

                mpData = 0;
            }
        /// @}


        /// @name Access
        /// @{
            Variables_List::Pointer pGetVariablesList()
            {
                return mpVariablesList;
            }
            const Variables_List::Pointer pGetVariablesList() const
            {
                return mpVariablesList;
            }
            Variables_List& GetVariablesList()
            {
                return *mpVariablesList;
            }
            const Variables_List& GetVariablesList() const
            {
                return *mpVariablesList;
            }
            
            BlockType* Data()
            {
                return mpData;
            }
            const BlockType* Data() const
            {
                return mpData;
            }
            BlockType* Data(SizeType QueueIndex)
            {
                return Position(QueueIndex);
            }
            BlockType* Data(Variable_Data const & rThisVariable)
            {
                if(!mpVariablesList->Has(rThisVariable)) 
                {
                    std::cerr << "Variable " << rThisVariable.Name() << " is not added to this variables list. Stopping" << std::endl;
                    exit(0);
                }
                return Position(rThisVariable);
            }


            template<class TDataType>
            TDataType& GetValue(const Variable<TDataType>& rThisVariable)
            {
                return *(reinterpret_cast<TDataType*>(Position(rThisVariable)) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            TDataType& GetValue(const Variable<TDataType>& rThisVariable, SizeType QueueIndex)
            {
                return *(reinterpret_cast<TDataType*>(Position(rThisVariable, QueueIndex)) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            const TDataType& GetValue(const Variable<TDataType>& rThisVariable) const
            {
                return *(reinterpret_cast<const TDataType*>(Position(rThisVariable)) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            const TDataType& GetValue(const Variable<TDataType>& rThisVariable, SizeType QueueIndex) const
            {
                return *(reinterpret_cast<const TDataType*>(Position(rThisVariable, QueueIndex)) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            TDataType& FastGetValue(const Variable<TDataType>& rThisVariable)
            {
                return *(reinterpret_cast<TDataType*>(Position(rThisVariable)) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            TDataType* pFastGetValue(const Variable<TDataType>& rThisVariable)
            {
                return (reinterpret_cast<TDataType*>(Position(rThisVariable)) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            TDataType& FastGetValue(const Variable<TDataType>& rThisVariable, SizeType QueueIndex)
            {
                return *(reinterpret_cast<TDataType*>(Position(rThisVariable, QueueIndex)) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            TDataType& FastGetValue(const Variable<TDataType>& rThisVariable, SizeType QueueIndex, SizeType ThisPosition)
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                if(!mpVariablesList->Has(rThisVariable)) 
                {
                    std::cerr<< "This container only can store the variables specified in its variables list. The variables list doesn't have this variable:" << rThisVariable << std::endl;
                    exit(0);
                }
                if((QueueIndex + 1) > mQueueSize) 
                {
                    std::cerr<< "Trying to access data from step " << QueueIndex << " but only " << mQueueSize << " steps are stored." << std::endl;
                    exit(0);
                }
                return *(reinterpret_cast<TDataType*>(Position(QueueIndex) + ThisPosition) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            TDataType& FastGetCurrentValue(const Variable<TDataType>& rThisVariable, SizeType ThisPosition)
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                if(!mpVariablesList->Has(rThisVariable)) 
                {
                    std::cerr<< "This container only can store the variables specified in its variables list. The variables list doesn't have this variable:" << rThisVariable << std::endl;
                    exit(0);
                }
                return *(reinterpret_cast<TDataType*>(mpCurrentPosition + ThisPosition) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            const TDataType& FastGetValue(const Variable<TDataType>& rThisVariable) const
            {
                return *(reinterpret_cast<const TDataType*>(Position(rThisVariable)) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            const TDataType* pFastGetValue(const Variable<TDataType>& rThisVariable) const
            {
                return (reinterpret_cast<const TDataType*>(Position(rThisVariable)) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            const TDataType& FastGetValue(const Variable<TDataType>& rThisVariable, SizeType QueueIndex) const
            {
                return *(reinterpret_cast<const TDataType*>(Position(rThisVariable, QueueIndex)) + rThisVariable.GetComponentIndex());
            }
            template<class TDataType>
            const TDataType& FastGetValue(const Variable<TDataType>& rThisVariable, SizeType QueueIndex, SizeType ThisPosition) const
            {
                return *(reinterpret_cast<const TDataType*>(Position(QueueIndex) + ThisPosition) + rThisVariable.GetComponentIndex());
            }

            template<class TDataType>
            const TDataType& FastGetCurrentValue(const Variable<TDataType>& rThisVariable, SizeType ThisPosition) const
            {
                return *(reinterpret_cast<TDataType*>(mpCurrentPosition + ThisPosition) + rThisVariable.GetComponentIndex());
            }
         
            template<class TDataType> 
            void SetValue(const Variable<TDataType>& rThisVariable, TDataType const& rValue)
            {
                GetValue(rThisVariable) = rValue;
            }

            template<class TDataType> 
            void SetValue(const Variable<TDataType>& rThisVariable, TDataType const& rValue, SizeType QueueIndex)
            {
                GetValue(rThisVariable, QueueIndex) = rValue;
            }
        /// @}


        /// @name Inquiry
        /// @{
            SizeType Size() const
            {
                if(!mpVariablesList)
                    return 0;

                return mpVariablesList->DataSize();
            }
            SizeType QueueSize() const
            {
                return mQueueSize;
            }
            SizeType TotalSize() const
            {
                if(!mpVariablesList)
                    return 0;

                return mQueueSize * mpVariablesList->DataSize();
            }
            SizeType DataSize()
            {
                if(!mpVariablesList)
                    return 0;

                return mpVariablesList->DataSize() * sizeof(BlockType);
            }
            bool Has(const Variable_Data& rThisVariable) const
            {
                if(!mpVariablesList)
                    return false;
                return mpVariablesList->Has(rThisVariable);
            }
            bool IsEmpty()
            {
                if(!mpVariablesList)
                    return true;
                return mpVariablesList->IsEmpty();
            }

        /// @}

        /// @name Input and Output
        /// @{
            std::string Info() const
            {
                return std::string("variables list data value container");
            }
            void PrintInfo(std::ostream& rOStream) const
            {
                rOStream << "variables list data value container";
            }
            void PrintData(std::ostream& rOStream) const
            {
                if(!mpVariablesList)
                    rOStream << "No varaibles list is assigned yet." << std::endl;
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    rOStream <<"    ";
                    for(SizeType i = 0 ; i < mQueueSize ; i++) {
                        rOStream << i << ": ";
                        it_variable->Print(Position(*it_variable, i), rOStream);
                        rOStream << "  ";
                    }
                    rOStream << std::endl;
                }

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
            SizeType mQueueSize;

            BlockType* mpCurrentPosition;

            ContainerType mpData;

            Variables_List::Pointer mpVariablesList;
        /// @}


        /// @name Private Operatiors
        /// @{


        /// @}


        /// @name Private Operations
        /// @{
            inline void Allocate()
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                mpData = (BlockType*)malloc(mpVariablesList->DataSize() * sizeof(BlockType) * mQueueSize);
            }
            inline void Reallocate()
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                mpData = (BlockType*)realloc(mpData, mpVariablesList->DataSize() * sizeof(BlockType) * mQueueSize);
            }
            void DestructElements(SizeType ThisIndex)
            {
                if(!mpVariablesList)
                    return;
                if(mpData == 0)
                    return;
                BlockType* position = Position(ThisIndex);
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    it_variable->Destruct(position + LocalOffset(*it_variable));
                }
            }
            void DestructAllElements()
            {
                if(!mpVariablesList)
                    return;

                if(mpData == 0)
                    return;

                const SizeType size = mpVariablesList->DataSize();
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    BlockType*  position = mpData + LocalOffset(*it_variable);
                    for(SizeType i = 0 ; i < mQueueSize ; i++) {
                        it_variable->Destruct(position + i * size);
                    }
                }
            }

            void AssignData(BlockType* Source, BlockType* Destination)
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                for(Variables_List::const_iterator it_variable = mpVariablesList->begin(); it_variable != mpVariablesList->end() ; it_variable++) {
                    const SizeType offset = LocalOffset(*it_variable);
                    it_variable->Assign(Source + offset, Destination + offset);
                }
            }
            

        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{
            inline SizeType LocalOffset(Variable_Data const & rThisVariable) const
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                return mpVariablesList->Index(rThisVariable.SourceKey());
            }
            inline BlockType* Position(Variable_Data const & rThisVariable) const
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                if(!mpVariablesList->Has(rThisVariable)) 
                {
                    std::cerr<< "This container only can store the variables specified in its variables list. The variables list doesn't have this variable:" << rThisVariable << std::endl;
                    exit(0);
                }
                return mpCurrentPosition + mpVariablesList->Index(rThisVariable.SourceKey());
            }

            inline BlockType* Position(Variable_Data const & rThisVariable, SizeType ThisIndex) const
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                if(!mpVariablesList->Has(rThisVariable)) 
                {
                    std::cerr<< "This container only can store the variables specified in its variables list. The variables list doesn't have this variable:" << rThisVariable << std::endl;
                    exit(0);
                }
                if((ThisIndex + 1) > mQueueSize) 
                {
                    std::cerr<< "Trying to access data from step " << ThisIndex << " but only " << mQueueSize << " steps are stored." << std::endl;
                    exit(0);
                }
                return Position(ThisIndex) + mpVariablesList->Index(rThisVariable.SourceKey());
            }

            inline BlockType* Position() const
            {
                return mpCurrentPosition;
            }

            inline BlockType* Position(SizeType ThisIndex) const
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                const SizeType total_size = TotalSize();
                BlockType* position = mpCurrentPosition + ThisIndex * mpVariablesList->DataSize();
                return (position < mpData + total_size) ? position : position - total_size;
            }
        /// @}
};
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Variables_List_Data_Value_Container& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);
    return rOStream;
}
#endif