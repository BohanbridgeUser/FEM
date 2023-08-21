#ifndef _NODE_Data_H_
#define _NODE_Data_H_

#include "../define.h"
#include <vector>

class Node_Data
{
    public:
        ///@name Define 
        ///@{
        LOTUS_POINTER_DEFINE(Node_Data)
        typedef std::vector<double> SolutionOneStepNodalDataContainerType;
        typedef std::vector<SolutionOneStepNodalDataContainerType> SolutionStepsNodalDataContainerType;
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
            SolutionStepsNodalDataContainerType& GetSolutionStepsNodalData()
            {
                return mSolutionStepsNodalData;
            }
            SolutionStepsNodalDataContainerType GetSolutionStepsNodalData()const
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