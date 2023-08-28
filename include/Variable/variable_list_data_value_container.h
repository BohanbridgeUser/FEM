#ifndef _VARIABLE_LIST_DATA_VALUE_CONTAINER_H_
#define _VARIABLE_LIST_DATA_VALUE_CONTAINER_H_

#include "variable.h"
#include "variables_list.h"

class Variable_List_Data_Value_Container
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
            Variable_List_Data_Value_Container()
            {

            }
            
            virtual ~Variable_List_Data_Value_Container()
            {
                
            }
        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{


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

        /// @}


        /// @name Inquiry
        /// @{


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
            inline BlockType* Position(Variable_Data const & rThisVariable) const
            {
                if(!mpVariablesList) 
                {
                    std::cerr<< "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                    exit(0);
                }
                if(mpVariablesList->Has(rThisVariable)) 
                {
                    std::cerr<< "This container only can store the variables specified in its variables list. The variables list doesn't have this variable:" << rThisVariable << std::endl;
                    exit(0);
                }
                return mpCurrentPosition + mpVariablesList->Index(rThisVariable.SourceKey());
            }

            inline BlockType* Position(Variable_Data const & rThisVariable, SizeType ThisIndex) const
            {
                // KRATOS_DEBUG_ERROR_IF(!mpVariablesList) << "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                // KRATOS_DEBUG_ERROR_IF_NOT(mpVariablesList->Has(rThisVariable)) << "This container only can store the variables specified in its variables list. The variables list doesn't have this variable:" << rThisVariable << std::endl;
                // KRATOS_DEBUG_ERROR_IF((ThisIndex + 1) > mQueueSize) << "Trying to access data from step " << ThisIndex << " but only " << mQueueSize << " steps are stored." << std::endl;
                return Position(ThisIndex) + mpVariablesList->Index(rThisVariable.SourceKey());
            }

            inline BlockType* Position() const
            {
                return mpCurrentPosition;
            }

            inline BlockType* Position(SizeType ThisIndex) const
            {
                // KRATOS_DEBUG_ERROR_IF(!mpVariablesList) << "This container don't have a variables list assigned. A possible reason is creating a node without a model part." << std::endl;
                const SizeType total_size = TotalSize();
                BlockType* position = mpCurrentPosition + ThisIndex * mpVariablesList->DataSize();
                return (position < mpData + total_size) ? position : position - total_size;
            }

        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{
            SizeType DataSize()
            {
                if(!mpVariablesList)
                    return 0;

                return mpVariablesList->DataSize() * sizeof(BlockType);
            }
            SizeType TotalSize() const
            {
                if(!mpVariablesList)
                    return 0;

                return mQueueSize * mpVariablesList->DataSize();
            }

        /// @}
};
#endif