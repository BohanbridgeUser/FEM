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
            const Variable_Data& GetVariable() const
            {
                return mNodeData->GetSolutionStepData().GetVariablesList().GetDofVariable(mID);
            }
            double& GetSolutionStepValue(const IndexType& rThisSolutionStep =0)
            {
                return GetReference(GetVariable(),mNodeData->GetSolutionStepData(),rThisSolutionStep,mReactionType);
            }
            double const& GetSolutionStepValue(const IndexType& rThisSolutionStep = 0)const
            {
                return GetReference(GetVariable(),mNodeData->GetSolutionStepData(),rThisSolutionStep,mReactionType);
            }
        /// @}

        /// @name Inquiry
        /// @{
            bool IsFree()
            {
                return mIsFixed != 1;
            }
            bool IsFixed()
            {
                return mIsFixed == 1;
            }
        /// @}
    private:
        /// @name Private Static Member
        /// @{

        /// @}

        /// @name Private Member
        /// @{
            IndexType mID;
            int mIsFixed : 1;
            int mVariableType : 4;
            int mReactionType : 4;
            EquationType mEQID;
            NodeDataType* mNodeData;
        /// @}
        
        /// @name Private Operators
        /// @{

        /// @}

        /// @name Private Operations
        /// @{
            double& GetReference(const Variable_Data& rThisVariableData, Variables_List_Data_Value_Container& rData,const IndexType& rThisSolutionStep, const int& rReactionType)
            {
                if(!rReactionType) 
                    return rData.GetValue(static_cast< Variable<double> const&>(rThisVariableData),rThisSolutionStep);
                else
                {
                    std::cerr << "Not supported type for Dof" << std::endl;
                    exit(0);
                }
            }
            double const& GetReference(const Variable_Data& rThisVariableData,const Variables_List_Data_Value_Container& rData,const IndexType& rThisSolutionStep, const int& rReactionType)const
            {
                if(!rReactionType) return rData.GetValue(static_cast< Variable<double> const&>(rThisVariableData),rThisSolutionStep);
                else
                {
                    std::cerr << "Not supported type for Dof" << std::endl;
                    exit(0);
                }
            }
        /// @}
};
#endif