#ifndef _NODE_H_
#define _NODE_H_
#include "../Geometry/Point.h"
#include "node_data.h"
#include "dof.h"
#include "../Container/data_value_container.h"

#include <vector>
#include <algorithm>
class Node : public Point<3> , public Flags
{
    public:
        ///@name Define
        ///@{ 
            LOTUS_SHARED_POINTER_DEFINE(Node)
            typedef Node                    
                                                        NodeType;
            typedef Point<3> 
                                                        BaseType;
            typedef double
                                                       BlockType;
            typedef Point<3> 
                                                       PointType;
            typedef Node_Data 
                                                    NodeDataType;
            typedef Dof<double>
                                                         DofType;
            typedef std::vector<std::unique_ptr<Dof<double>>> 
                                        DofPointersContainerType;
            typedef size_t
                                                       IndexType;
            typedef Variables_List_Data_Value_Container
                                 VariablesListDataValueContainer; 
        ///@}

        ///@name Life Circle
        ///@{
            // Constructor
            /// Default constructor.
            Node()
            :BaseType()
            ,Flags()
            ,mNodeData(0)
            ,mDofsContainer()
            ,mData()
            ,mInitialPosition()
            {
                CreateSolutionStepData();
            }
            explicit Node(IndexType NewId )
            : BaseType()
            , Flags()
            , mNodeData(NewId)
            , mDofsContainer()
            , mData()
            , mInitialPosition()
            {
                std::cerr <<  "Calling the default constructor for the node ... illegal operation!!" << std::endl;
                exit(0);
                CreateSolutionStepData();
            }
            /// 3d constructor.
            Node(IndexType NewId,
                 double const& NewX, 
                 double const& NewY, 
                 double const& NewZ)
            : BaseType(NewX, NewY, NewZ)
            , Flags()
            , mNodeData(NewId)
            , mDofsContainer()
            , mData()
            , mInitialPosition(NewX, NewY, NewZ)
            {
                CreateSolutionStepData();
            }
            /// Point constructor.
            Node(IndexType NewId, PointType const& rThisPoint)
            : BaseType(rThisPoint)
            , Flags()
            , mNodeData(NewId)
            , mDofsContainer()
            , mData()
            , mInitialPosition(rThisPoint)
            {
                CreateSolutionStepData();
            }
            /** Copy constructor. Initialize this node with given node.*/
            Node(Node const& rOtherNode) = delete;
            /// 3d with variables list and data constructor.
            Node(IndexType NewId, double const& NewX, double const& NewY, double const& NewZ,
                 Variables_List::Pointer  pVariablesList,
                 BlockType const * ThisData, 
                 SizeType NewQueueSize = 1)
            : BaseType(NewX, NewY, NewZ)
            , Flags()
            , mNodeData(NewId, pVariablesList,ThisData,NewQueueSize)
            , mDofsContainer()
            , mData()
            , mInitialPosition(NewX, NewY, NewZ)
            {
            }
            // Destructor
            ~Node()
            {

            }    
        ///@}

        /// @name Operators
        /// @{
            Node& operator=(const Node& rOther)
            {
                BaseType::operator=(rOther);
                Flags::operator =(rOther);

                mNodeData = rOther.mNodeData;

                // Deep copying the dofs
                for(typename DofPointersContainerType::const_iterator it_dof = rOther.mDofsContainer.begin() ; it_dof != rOther.mDofsContainer.end() ; it_dof++)
                {
                    pAddDof(**it_dof);
                }

                mData = rOther.mData;
                mInitialPosition = rOther.mInitialPosition;

                return *this;
            }
            Node& operator=(Node&& rOther)
            {
                BaseType::operator=(rOther);
                Flags::operator =(rOther);

                mNodeData = rOther.mNodeData;

                // Deep copying the dofs
                for(typename DofPointersContainerType::const_iterator it_dof = rOther.mDofsContainer.begin() ; it_dof != rOther.mDofsContainer.end() ; it_dof++)
                {
                    pAddDof(**it_dof);
                }

                mData = rOther.mData;
                mInitialPosition = rOther.mInitialPosition;

                return *this;
            }
        /// @}
        
        ///@name Operations {
            void CreateSolutionStepData()
            {
                SolutionStepData().PushFront();
            }

            void CloneSolutionStepData()
            {
                SolutionStepData().CloneFront();
            }

            void OverwriteSolutionStepData(IndexType SourceSolutionStepIndex, IndexType DestinationSourceSolutionStepIndex)
            {
                SolutionStepData().AssignData(SolutionStepData().Data(SourceSolutionStepIndex), DestinationSourceSolutionStepIndex);
            }

            void ClearSolutionStepsData()
            {
                SolutionStepData().Clear();
            }

            void SetSolutionStepVariablesList(Variables_List::Pointer pVariablesList)
            {
                SolutionStepData().SetVariablesList(pVariablesList);
            }

            IndexType GetBufferSize() const
            {
                return SolutionStepData().QueueSize();
            }

            void SetBufferSize(IndexType NewBufferSize)
            {
                SolutionStepData().Resize(NewBufferSize);
            }
        
            /** adds a Dof to the node and return new added dof or existed one. */
            template<class TVariableType>
            inline typename DofType::Pointer pAddDof(TVariableType const& rDofVariable)
            {
                for(auto it_dof = mDofsContainer.begin() ; it_dof != mDofsContainer.end() ; it_dof++){
                    if((*it_dof)->GetVariable() == rDofVariable){
                        return (*it_dof).get();
                    }
                }
                mDofsContainer.push_back(std::make_unique<DofType>(&mNodeData, rDofVariable));
                DofType* p_new_dof = mDofsContainer.back().get();
                SortDofs();

                return p_new_dof;
            }
            /** adds a Dof to the node and return new added dof or existed one. */
            inline typename DofType::Pointer pAddDof(DofType const& SourceDof)
            {
                for(auto it_dof = mDofsContainer.begin() ; it_dof != mDofsContainer.end() ; it_dof++){
                    if((*it_dof)->GetVariable() == SourceDof.GetVariable()){
                        if(!((*it_dof)->GetReaction() == SourceDof.GetReaction()))
                        {
                            **it_dof = SourceDof;
                            (*it_dof)->SetNodalData(&mNodeData);
                        }
                        return (*it_dof).get();
                    }
                }
                mDofsContainer.push_back(std::make_unique<DofType>(SourceDof));
                mDofsContainer.back()->SetNodalData(&mNodeData);
                DofType* p_new_dof = mDofsContainer.back().get();
                SortDofs();
                return p_new_dof;
            }
            /** adds a Dof to the node and return new added dof or existed one. */
            template<class TVariableType, class TReactionType>
            inline typename DofType::Pointer pAddDof(TVariableType const& rDofVariable, TReactionType const& rDofReaction)
            {
                for(auto it_dof = mDofsContainer.begin() ; it_dof != mDofsContainer.end() ; it_dof++){
                    if((*it_dof)->GetVariable() == rDofVariable){
                        (*it_dof)->SetReaction(rDofReaction);
                        return (*it_dof).get();
                    }
                }
                mDofsContainer.push_back(std::make_unique<DofType>(&mNodeData, rDofVariable, rDofReaction));
                DofType* p_new_dof = mDofsContainer.back().get();
                SortDofs();
                return p_new_dof;
            }
            /** adds a Dof to the node and return new added dof or existed one. */
            template<class TVariableType>
            inline DofType& AddDof(TVariableType const& rDofVariable)
            {
                for(auto it_dof = mDofsContainer.begin() ; it_dof != mDofsContainer.end() ; it_dof++){
                    if((*it_dof)->GetVariable() == rDofVariable){
                        return **it_dof;
                    }
                }
                mDofsContainer.push_back(std::make_unique<DofType>(&mNodeData, rDofVariable));
                DofType* p_new_dof = mDofsContainer.back().get();
                SortDofs();
                return *p_new_dof;
            }
            /** adds a Dof to the node and return new added dof or existed one. */
            template<class TVariableType, class TReactionType>
            inline DofType& AddDof(TVariableType const& rDofVariable, TReactionType const& rDofReaction)
            {
                for(auto it_dof = mDofsContainer.begin() ; it_dof != mDofsContainer.end() ; it_dof++){
                    if((*it_dof)->GetVariable() == rDofVariable){
                        (*it_dof)->SetReaction(rDofReaction);
                        return **it_dof;
                    }
                }
                mDofsContainer.push_back(std::make_unique<DofType>(&mNodeData, rDofVariable, rDofReaction));
                DofType* p_new_dof = mDofsContainer.back().get();
                SortDofs();
                return *p_new_dof;
            }

        ///@}

        /// @name Access
        /// @{
            IndexType GetNodeID()const
            {
                return mNodeData.Id();
            }
            IndexType& Id()
            {
                return mNodeData.Id();
            }
            IndexType Id() const
            {
                return mNodeData.Id();
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
            const DofPointersContainerType& GetDofs() const
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

             /**
             * @brief Get dof with a given position. If not found it is search
             * @param rDofVariable Name of the variable
             * @param pos Position of the DoF
             * @tparam TVariableType The variable type template argument
             * @return The DoF associated to the given variable
             */
            template<class TVariableType>
            inline const DofType& GetDof(TVariableType const& rDofVariable, int pos) const
            {
                typename DofPointersContainerType::const_iterator it_begin = mDofsContainer.begin();
                typename DofPointersContainerType::const_iterator it_end = mDofsContainer.end();
                typename DofPointersContainerType::const_iterator it;
                //if the guess is exact return the guess
                if(pos < it_end-it_begin)
                {
                    it = it_begin + pos;
                    if( (*it)->GetVariable() == rDofVariable)
                    {
                        return **it;
                    }
                }

                // Otherwise do a find
                for(auto it_dof = mDofsContainer.begin() ; it_dof != mDofsContainer.end() ; it_dof++){
                    if((*it_dof)->GetVariable() == rDofVariable){
                        return **it_dof;
                    }
                }
                std::cerr <<  "Non-existent DOF in node #" << Id() << " for variable : " << rDofVariable.Name() << std::endl;
                exit(0);
            }
            /**
             * @brief Get DoF for a given variable
             * @param rDofVariable Name of the variable
             * @tparam TVariableType The variable type template argument
             * @return The DoF associated to the given variable
             */
            template<class TVariableType>
            inline const DofType& GetDof(TVariableType const& rDofVariable) const
            {
                for(auto it_dof = mDofsContainer.begin() ; it_dof != mDofsContainer.end() ; it_dof++){
                    if((*it_dof)->GetVariable() == rDofVariable){
                        return **it_dof;
                    }
                }
                std::cerr <<  "Non-existent DOF in node #" << Id() << " for variable : " << rDofVariable.Name() << std::endl;
                exit(0);
            }
           /**
             * @brief Get DoF counted pointer for a given variable
             * @param rDofVariable Name of the variable
             * @tparam TVariableType The variable type template argument
             * @return The DoF associated to the given variable
             */
            template<class TVariableType>
            inline typename DofType::Pointer pGetDof(TVariableType const& rDofVariable) const
            {
                for(auto it_dof = mDofsContainer.begin() ; it_dof != mDofsContainer.end() ; it_dof++){
                    if((*it_dof)->GetVariable() == rDofVariable){
                        return (*it_dof).get();
                    }
                }
                std::cerr <<  "Non-existent DOF in node #" << Id() << " for variable : " << rDofVariable.Name() << std::endl;
                exit(0);
            }
            /**
             * @brief Get DoF counted pointer with a given position. If not found it is search
             * @param rDofVariable Name of the variable
             * @param Position Position of the DoF
             * @tparam TVariableType The variable type template argument
             * @return The DoF associated to the given variable
             */
            template<class TVariableType>
            inline typename DofType::Pointer pGetDof(
                TVariableType const& rDofVariable,
                int Position
                ) const
            {
                const auto it_begin = mDofsContainer.begin();
                const auto it_end = mDofsContainer.end();
                // If the guess is exact return the guess
                if(Position < it_end-it_begin) {
                    auto it_dof = it_begin + Position;
                    if( (*it_dof)->GetVariable() == rDofVariable) {
                        return (*it_dof).get();
                    }
                }

                // Otherwise do a find
                for(auto it_dof = it_begin; it_dof != it_end; ++it_dof){
                    if((*it_dof)->GetVariable() == rDofVariable){
                        return (*it_dof).get();
                    }
                }

                std::cerr <<  "Non-existent DOF in node #" << Id() << " for variable : " << rDofVariable.Name() << std::endl;
                exit(0);
            }

            
            /**
             * @name GetSolutionStepValue
             * @brief These methods can get values of variable
            */
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

            /*********************************************************************************************/
            /**
             * @name FastGetSolutionStepValue
             * @brief An Interface to Data_Value_Container's FastGetSolutionStepValue
            */
            template<class TVariableType> 
            typename TVariableType::Type& FastGetSolutionStepValue(const TVariableType& rThisVariable)
            {
                return SolutionStepData().FastGetValue(rThisVariable);
            }
            template<class TVariableType> 
            const typename TVariableType::Type& FastGetSolutionStepValue(const TVariableType& rThisVariable) const
            {
                return SolutionStepData().FastGetValue(rThisVariable);
            }
            template<class TVariableType> 
            typename TVariableType::Type& FastGetSolutionStepValue(const TVariableType& rThisVariable,
                                                                   IndexType SolutionStepIndex)
            {
                return SolutionStepData().FastGetValue(rThisVariable, SolutionStepIndex);
            }
            template<class TVariableType> 
            const typename TVariableType::Type& FastGetSolutionStepValue(const TVariableType& rThisVariable,
                                                                         IndexType SolutionStepIndex) const
            {
                return SolutionStepData().FastGetValue(rThisVariable, SolutionStepIndex);
            }
            template<class TVariableType> 
            typename TVariableType::Type& FastGetSolutionStepValue(const TVariableType& rThisVariable,
                                                                   IndexType SolutionStepIndex, IndexType ThisPosition)
            {
                return SolutionStepData().FastGetValue(rThisVariable, SolutionStepIndex, ThisPosition);
            }
            template<class TVariableType> 
            typename TVariableType::Type& FastGetCurrentSolutionStepValue(const TVariableType& rThisVariable,
                                                                          IndexType ThisPosition)
            {
                return SolutionStepData().FastGetCurrentValue(rThisVariable, ThisPosition);
            }
            /*********************************************************************************************/

            template<class TVariableType>
            typename TVariableType::Type& GetValue(const TVariableType& rThisVariable)
            {
                return mData.GetValue(rThisVariable);
            }
            template<class TVariableType>
            typename TVariableType::Type const& GetValue(const TVariableType& rThisVariable) const
            {
                return mData.GetValue(rThisVariable);
            }
            template<class TVariableType>
            typename TVariableType::Type& GetValue(const TVariableType& rThisVariable, const IndexType rThisSolutionStep)
            {
                return mData.GetValue(rThisVariable,rThisSolutionStep);
            }
            template<class TVariableType>
            typename TVariableType::Type const& GetValue(const TVariableType& rThisVariable, const IndexType rThisSolutionStep) const
            {
                return mData.GetValue(rThisVariable,rThisSolutionStep);
            }

            Lotus_Vector<3> GetInitialPosition()const
            {
                return mInitialPosition;
            }
            Lotus_Vector<3>& GetInitialPosition()
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

            inline  bool IsFixed(const Variable_Data& rDofVariable) const
            {
                for(auto it_dof = mDofsContainer.begin() ; it_dof != mDofsContainer.end() ; it_dof++){
                    if((*it_dof)->GetVariable() == rDofVariable){
                        return (*it_dof)->IsFixed();
                    }
                }
                return false;
            }
            template<typename TDataType>
            inline bool SolutionStepsDataHas(const Variable<TDataType>& rVariable) const
            {
                return mNodeData.SolutionStepsDataHas(rVariable);
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

        Lotus_Vector<3> mInitialPosition;

        Data_Value_Container mData;


        void SortDofs()
        {
            std::sort(mDofsContainer.begin(), mDofsContainer.end(), [](std::unique_ptr<DofType> const& First, std::unique_ptr<DofType> const& Second)->bool
            {
                return First->GetVariable().Key() < Second->GetVariable().Key();
            });
        }
};

#endif