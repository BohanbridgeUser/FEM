#include "../../include/Node/node_data.h"

int Node_Data::nodenum = 0;

Node_Data::Node_Data(IndexType TheId)
: nodeID(TheId)
, mSolutionStepsNodalData()
{

}

Node_Data::Node_Data(IndexType TheId,
                     Variables_List::Pointer pVariablesList, 
                     SizeType NewQueueSize)
: nodeID(TheId)
, mSolutionStepsNodalData(pVariablesList,NewQueueSize)
{

}

Node_Data::Node_Data(IndexType TheId, 
                     Variables_List::Pointer pVariablesList, 
                     BlockType const * ThisData, 
                     SizeType NewQueueSize)
: nodeID(TheId)
, mSolutionStepsNodalData(pVariablesList,ThisData,NewQueueSize)
{

}