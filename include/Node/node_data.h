#ifndef _NODE_Data_H_
#define _NODE_Data_H_

#include "../define.h"
#include "../Variable/variable_list_data_value_container.h"

#include <vector>

class Node_Data
{
    public:
        ///@name Define 
        ///@{
        LOTUS_POINTER_DEFINE(Node_Data)
        typedef std::vector<double> SolutionOneStepNodalDataContainerType;
        typedef Variable_List_Data_Value_Container SolutionStepsNodalDataContainerType;
        ///@}

        ///@name Life circle 
        ///@{    
            // Contructor
            Node_Data()
            {
                nodenum++;
                nodeID = nodenum;
            }
            Node_Data(const int& ID):nodeID(ID)
            {
                nodenum++;
            }     
            Node_Data(const int& ID, const SolutionStepsNodalDataContainerType& ThisSolution)
            :nodeID(ID),
            mSolutionStepsNodalData(ThisSolution)
            {
                nodenum++;
            }   
            Node_Data(const Node_Data& another)
            :nodeID(another.nodeID),
            mSolutionStepsNodalData(another.mSolutionStepsNodalData)
            {
                nodenum++;
            }
        
            // Destructor 
            ~Node_Data()
            {
                if (nodenum>0) nodenum--;
            }    
        ///@}

        ///@name Operations {
            int GetID()const
            {
                return nodeID;
            }
            int& GetID()
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
    private:
        int nodeID;
        static int nodenum;
        SolutionStepsNodalDataContainerType mSolutionStepsNodalData;
};

#endif