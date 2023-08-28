#ifndef _NODE_H_
#define _NODE_H_
#include "../Geometry/Point.h"
#include "node_data.h"
#include "dof.h"
#include <vector>
class Node : public Point<3>
{
    public:
        ///@name Define
        ///@{ 
            LOTUS_POINTER_DEFINE(Node)
            typedef Node                    
                                                        NodeType;
            typedef Point<3> 
                                                        BaseType;
            typedef Point<3> 
                                                       PointType;
            typedef Node_Data 
                                                    NodeDataType;
            typedef std::vector<Dof*> 
                                        DofPointersContainerType;
            typedef Dof
                                                         DofType;
            typedef size_t
                                                       IndexType;
            typedef Variable_List_Data_Value_Container
                                 VariablesListDataValueContainer; 
        ///@}

        ///@name Life Circle
        ///@{
            // Constructor
            Node()
            :PointType(),
             mNodeData(),
             mDofsContainer(),
             mInitialPosition()
            {

            }
            Node(const int& ID)
            :PointType(),
             mNodeData(ID),
             mDofsContainer(),
             mInitialPosition()
            {

            }  
            Node(const double& x, const double& y, const double& z)
                :PointType(x,y,z),
                mNodeData(),
                mDofsContainer(),
                mInitialPosition(x,y,z)
            {

            }
            Node(const double& x, const double& y, const double& z,
                const NodeDataType& ThisNodeData, const DofPointersContainerType& ThisDofs)
                :PointType(x,y,z),
                mNodeData(ThisNodeData),
                mDofsContainer(ThisDofs),
                mInitialPosition(x,y,z)
            {
                
            }  
            Node(const Node& another)
                :PointType(another.x(),another.y(),another.z()),
                mNodeData(another.mNodeData),
                mDofsContainer(another.mDofsContainer),
                mInitialPosition(another.x(),another.y(),another.z())
            {

            }
            Node(Node&& another)
                :PointType(another.x(),another.y(),another.z()),
                mNodeData(another.mNodeData),
                mDofsContainer(another.mDofsContainer),
                mInitialPosition(another.x(),another.y(),another.z())
            {

            }
            Node(const Point<3>& anotherP)
                 :PointType(anotherP),
                 mNodeData(),
                 mDofsContainer(),
                 mInitialPosition(anotherP)
            {

            }
            Node(const Point<3>& anotherP,
                 const NodeDataType& ThisNodeData, 
                 const DofPointersContainerType& ThisDofs)
                 :PointType(anotherP),
                 mNodeData(ThisNodeData),
                 mDofsContainer(ThisDofs),
                 mInitialPosition(anotherP)
            {

            }
            // Destructor
            ~Node()
            {

            }    
        ///@}

        /// @name Operators
        /// @{
            Node& operator=(const Node& another)
            {
                this->x() = another.x();
                this->y() = another.y();
                this->z() = another.z();
                mNodeData = another.mNodeData;
                mNodeData.GetID()++;
                mDofsContainer = another.mDofsContainer;
                mInitialPosition = another.mInitialPosition;
                return *this;
            }
            Node& operator=(Node&& another)
            {
                this->x() = another.x();
                this->y() = another.y();
                this->z() = another.z();
                mDofsContainer = another.mDofsContainer;
                mInitialPosition = another.mInitialPosition;
                return *this;
            }
        /// @}
        
        ///@name Operations {
            
        ///@}

        /// @name Access
        /// @{
            int GetNodeID()const
            {
                return mNodeData.GetID();
            }
            NodeDataType& GetNodeData()
            {
                return mNodeData;
            }
            NodeDataType GetNodeData()const
            {
                return mNodeData;
            }
            DofPointersContainerType& GetDofs()
            {
                return mDofsContainer;
            }
            DofPointersContainerType GetDofs()const
            {
                return mDofsContainer;
            }
            VariablesListDataValueContainer& SolutionStepData()
            {
                return mNodeData.GetSolutionStepData();
            }
            const VariablesListDataValueContainer& SolutionStepData() const
            {
                return mNodeData.GetSolutionStepData();
            }

            template<typename TVariableType>
            typename Dof::Pointer pGetDof(const TVariableType& rDofVariable) const
            {
                for(auto it_dof = mDofsContainer.begin();it_dof != mDofsContainer.end();it_dof++)
                {
                    if((*it_dof)->GetVariable() == rDofVariable){
                        return (*it_dof);
                    }
                }
                std::cerr << "Dof Not Find!\n";
                return nullptr;
            }
            template<typename TVariableType>
            Dof& GetDof(const TVariableType& rDofVariable) const
            {
                for(auto it_dof = mDofsContainer.begin();it_dof != mDofsContainer.end();it_dof++)
                {
                    if((*it_dof)->GetVariable() == rDofVariable){
                        return *(*it_dof);
                    }
                }
                std::cerr << "Dof Not Find!\n";
                exit(0);
            }

            template<class TVariableType> 
            typename TVariableType::Type& GetSolutionStepValue(const TVariableType& rThisVariable)
            {
                return SolutionStepData().GetValue(rThisVariable);
            }

            template<class TVariableType> 
            typename TVariableType::Type const& GetSolutionStepValue(const TVariableType& rThisVariable) const
            {
                return SolutionStepData().GetValue(rThisVariable);
            }

            template<class TVariableType> 
            typename TVariableType::Type& GetSolutionStepValue(const TVariableType& rThisVariable,
                                                                int SolutionStepIndex)
            {
                return SolutionStepData().GetValue(rThisVariable, SolutionStepIndex);
            }

            template<class TVariableType> 
            typename TVariableType::Type const& GetSolutionStepValue(const TVariableType& rThisVariable,
                                                                     IndexType SolutionStepIndex) const
            {
                return SolutionStepData().GetValue(rThisVariable, SolutionStepIndex);
            }



            Vector<3> GetInitialPosition()const
            {
                return mInitialPosition;
            }
            Vector<3>& GetInitialPosition()
            {
                return mInitialPosition;
            }
        /// @}

        /// @name Inquiry
        /// @{
            inline bool HasDofFor(const Variable_Data& rDofVariable) const
            {
                for(auto it_dof = mDofsContainer.begin() ; it_dof != mDofsContainer.end() ; it_dof++){
                    if((*it_dof)->GetVariable() == rDofVariable){
                        return true;
                    }
                }
                return false;
            }
        /// @}

        /// @name Input and Output
        /// @{
            friend std::ostream& operator<<(std::ostream& os, const Node& another)
            {
                os << "Node ID:" << another.GetNodeID() 
                    << " Loc:" << another.x() << " "
                    << another.y() << " "
                    << another.z() << "\n";
                    return os;
            }
        /// @}
    private:
        // NodeData stores ID and SolutionData of Nodes
        NodeDataType mNodeData;

        DofPointersContainerType mDofsContainer;

        Vector<3> mInitialPosition;
};

#endif