#ifndef _NODE_Data_H_
#define _NODE_Data_H_

#include "../define.h"
#include "../Variable/variables_list_data_value_container.h"

#include <vector>

class Node_Data
{
    public:
        ///@name Define 
        ///@{
        LOTUS_POINTER_DEFINE(Node_Data)
        typedef size_t
                                                                                        IndexType;
        typedef size_t
                                                                                         SizeType;
        typedef double
                                                                                        BlockType;                                                                                 
        typedef std::vector<double> 
                                                            SolutionOneStepNodalDataContainerType;
        typedef Variables_List_Data_Value_Container 
                                                              SolutionStepsNodalDataContainerType;
        ///@}

        ///@name Life circle 
        ///@{    
            // Contructor
            Node_Data(IndexType TheId);

            Node_Data(IndexType TheId,
                      Variables_List::Pointer pVariablesList,
                      SizeType NewQueueSize = 1);

            Node_Data(IndexType TheId,
                      Variables_List::Pointer pVariablesList,
                      BlockType const * ThisData, 
                      SizeType NewQueueSize = 1);
            // Destructor 
            ~Node_Data()
            {
                if (nodenum>0) nodenum--;
            }    
        ///@}

        ///@name Operations {
            IndexType Id()const
            {
                return nodeID;
            }
            IndexType& Id()
            {
                return nodeID;
            }
            SolutionStepsNodalDataContainerType& GetSolutionStepData()
            {
                return mSolutionStepsNodalData;
            }
            const SolutionStepsNodalDataContainerType& GetSolutionStepData()const
            {
                return mSolutionStepsNodalData;
            }
        ///@}

        /// @name Inquiry
        /// @{
            template<typename TDataType>
            inline bool SolutionStepsDataHas(const Variable<TDataType>& rVariable) const
            {
                return mSolutionStepsNodalData.Has(rVariable);
            }
        /// @}
    private:
        IndexType nodeID;
        static int nodenum;
        SolutionStepsNodalDataContainerType mSolutionStepsNodalData;
};

#endif