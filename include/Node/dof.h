#ifndef _DOF_H_
#define _DOF_H_

#include "node.h"
#include "node_data.h"
class Dof
{
    public:
        ///@name Define 
        ///@{ 
            LOTUS_POINTER_DEFINE(Dof)
            typedef int IndexType;
            typedef int EquationType;
            typedef Node_Data NodeDataType;
        ///@}

        ///@ Lift Circle 
        ///@ {
            // Constructor  
            Dof()
            {

            }    
            Dof(const IndexType& id, const EquationType& eqid, NodeDataType& nodedata)
            :mID(id),
             mEQID(eqid),
             mNodeData(&nodedata)
            {

            }
            // Destructor 
            ~Dof()
            {

            }  
        ///@}

    private:
        IndexType mID;
        bool mIsFixed = false;
        EquationType mEQID;
        NodeDataType* mNodeData;
};
#endif