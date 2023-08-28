#ifndef _DOF_H_
#define _DOF_H_

#include "node.h"
#include "node_data.h"
#include "../Variable/variable.h"
class Dof
{
    public:
        ///@name Define 
        ///@{ 
            LOTUS_POINTER_DEFINE(Dof)
            typedef int                         
                                                    IndexType;
            typedef int                         
                                                 EquationType;
            typedef Node_Data                   
                                                 NodeDataType;
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

        /// @name Operators
        /// @{

        /// @}

        /// @name Operations
        /// @{
            const Variable_Data& GetVariable() const
            {
                return mNodeData->GetSolutionStepData().GetVariablesList().GetDofVariable(mID);
            }
        /// @}

        /// @name Access
        /// @{
            int Id()const
            {
                return mID;
            }
            EquationType EquationId() const
            {
                return mEQID;
            }
            EquationType& EquationId()
            {
                return mEQID;
            }
        /// @}
    private:
        IndexType mID;
        bool mIsFixed = false;
        EquationType mEQID;
        NodeDataType* mNodeData;
};
#endif