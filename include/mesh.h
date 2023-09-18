#ifndef _MESH_H_
#define _MESH_H_
#include "define.h"
#include "Container/pointer_vector_set.h"
#include "Container/pointer_vector_map.h"
#include "Container/flags.h"
#include "Container/data_value_container.h"
#include "Geometry/geometry.h"

#include <string>
#include <iostream>
#include <sstream>
#include <cstddef>
#include <unordered_map>
#include <vector>
template<typename TNodeType,
         typename TPropertiesType,
         typename TElementType,
         typename TConditionType>
class Mesh : public Data_Value_Container, public Flags
{
    public:
        ///@name Define 
        ///@{
            typedef Mesh<TNodeType,TPropertiesType,TElementType,TConditionType> 
                                                                        MeshType;
            LOTUS_POINTER_DEFINE(MeshType)

            typedef std::size_t 
                                                                       IndexType;
            typedef std::size_t 
                                                                        SizeType;
            typedef TNodeType 
                                                                        NodeType;
            typedef TPropertiesType 
                                                                  PropertiesType;
            typedef Geometry<NodeType> 
                                                                    GeometryType;
            typedef TElementType 
                                                                     ElementType;
            typedef TConditionType 
                                                                   ConditionType;

            //typedef std::unordered_map<int,NodeType,std::hash<int>> NodesContainerType;
            /// Nodes container. Which is a vector set of nodes with their Id's as key.
            typedef PointerVectorSet<NodeType,
                                    Indexed_Object,
                                    std::less<typename Indexed_Object::result_type>,
                                    std::equal_to<typename Indexed_Object::result_type>,
                                    typename NodeType::Pointer,
                                    std::vector< typename NodeType::Pointer >
                                    >                         NodesContainerType;
            /** Iterator over the nodes. This iterator is an indirect
            iterator over Node::Pointer which turn back a reference to
            node by * operator and not a pointer for more convenient
            usage. */
            typedef typename NodesContainerType::iterator 
                                                                    NodeIterator;
            /** Const iterator over the nodes. This iterator is an indirect
            iterator over Node::Pointer which turn back a reference to
            node by * operator and not a pointer for more convenient
            usage. */
            typedef typename NodesContainerType::const_iterator 
                                                            NodeConstantIterator;

            typedef PointerVectorSet<PropertiesType, Indexed_Object> 
                                                         PropertiesContainerType;
            /** Iterator over the properties. This iterator is an indirect
            iterator over Properties::Pointer which turn back a reference to
            properties by * operator and not a pointer for more convenient
            usage. */
            typedef typename PropertiesContainerType::iterator 
                                                              PropertiesIterator;
            /** Const iterator over the properties. This iterator is an indirect
            iterator over Properties::Pointer which turn back a reference to
            properties by * operator and not a pointer for more convenient
            usage. */
            typedef typename PropertiesContainerType::const_iterator 
                                                      PropertiesConstantIterator;


            /// Element container. A vector set of Elements with their Id's as key.
            typedef PointerVectorSet<ElementType,
                                    Indexed_Object,
                                    std::less<typename Indexed_Object::result_type>,
                                    std::equal_to<typename Indexed_Object::result_type>,
                                    typename ElementType::Pointer,
                                    std::vector< typename ElementType::Pointer >
                                    >                       ElementsContainerType;
            /** Iterator over the Elements. This iterator is an indirect
            iterator over Elements::Pointer which turn back a reference to
            Element by * operator and not a pointer for more convenient
            usage. */
            typedef typename ElementsContainerType::iterator 
                                                                  ElementIterator;
            /** Const iterator over the Elements. This iterator is an indirect
            iterator over Elements::Pointer which turn back a reference to
            Element by * operator and not a pointer for more convenient
            usage. */
            typedef typename ElementsContainerType::const_iterator 
                                                          ElementConstantIterator;

            /// Conditions container. A vector set of Conditions with their Id's as key.
            typedef PointerVectorSet<ConditionType,
                                    Indexed_Object,
                                    std::less<typename Indexed_Object::result_type>,
                                    std::equal_to<typename Indexed_Object::result_type>,
                                    typename ConditionType::Pointer,
                                    std::vector< typename ConditionType::Pointer >
                                    >                     ConditionsContainerType;
            /** Iterator over the Conditions. This iterator is an indirect
            iterator over Conditions::Pointer which turn back a reference to
            Condition by * operator and not a pointer for more convenient
            usage. */
            typedef typename ConditionsContainerType::iterator 
                                                                ConditionIterator;
            /** Const iterator over the Conditions. This iterator is an indirect
            iterator over Conditions::Pointer which turn back a reference to
            Condition by * operator and not a pointer for more convenient
            usage. */
            typedef typename ConditionsContainerType::const_iterator 
                                                        ConditionConstantIterator;

        ///@}

        ///@name Life Circle
        ///@{
            /// Default constructor.
            Mesh() : Flags()
                , mpNodes(new NodesContainerType())
                , mpProperties(new PropertiesContainerType())
                , mpElements(new ElementsContainerType())
                , mpConditions(new ConditionsContainerType())
                , mpMasterSlaveConstraints(new MasterSlaveConstraintContainerType())
            {}

            /// Copy constructor.
            Mesh(Mesh const& rOther) : Flags(rOther)
                , mpNodes(rOther.mpNodes)
                , mpProperties(rOther.mpProperties)
                , mpElements(rOther.mpElements)
                , mpConditions(rOther.mpConditions)
                , mpMasterSlaveConstraints(rOther.mpMasterSlaveConstraints) 
            {}

            /// Components constructor.
            Mesh(typename NodesContainerType::Pointer NewNodes,
                typename PropertiesContainerType::Pointer NewProperties,
                typename ElementsContainerType::Pointer NewElements,
                typename ConditionsContainerType::Pointer NewConditions,
                typename MasterSlaveConstraintContainerType::Pointer NewMasterSlaveConditions)
                : Flags(), mpNodes(NewNodes), mpProperties(NewProperties) , mpElements(NewElements), mpConditions(NewConditions), mpMasterSlaveConstraints(NewMasterSlaveConditions)
            {}


            /// Destructor.
            ~Mesh() override {}

        ///@}

        /// @name Operators
        /// @{

        /// @}

        /// @name Operations
        /// @{
            Mesh Clone()
            {
                typename NodesContainerType::Pointer p_nodes(new NodesContainerType(*mpNodes));
                typename PropertiesContainerType::Pointer p_properties(new PropertiesContainerType(*mpProperties));
                typename ElementsContainerType::Pointer p_elements(new ElementsContainerType(*mpElements));
                typename ConditionsContainerType::Pointer p_conditions(new ConditionsContainerType(*mpConditions));
                typename MasterSlaveConstraintContainerType::Pointer p_master_slave_constraints(new MasterSlaveConstraintContainerType(*mpMasterSlaveConstraints));

                return Mesh(p_nodes, p_properties, p_elements, p_conditions, p_master_slave_constraints);
            }

            void Clear()
            {
                Flags::Clear();
                DataValueContainer::Clear();
                mpNodes->clear();
                mpProperties->clear();
                mpElements->clear();
                mpConditions->clear();
                mpMasterSlaveConstraints->clear();
            }
        /// @}


        /// @name Inquiry
        /// @{
            SizeType WorkingSpaceDimension() const
            {
                SizeType dimension = 3;

                // NOTE: possible segmentacion fault if a Element or Condition
                // is created using the base class of geometry, then the mpGeometryData
                // of the geometry is a null pointer and has not any mWorkingSpaceDimension
                if(NumberOfElements()!=0)
                    dimension = (mpElements->begin())->GetGeometry().WorkingSpaceDimension();
                else if(NumberOfConditions()!=0)
                    dimension = (mpConditions->begin())->GetGeometry().WorkingSpaceDimension();
                else if(NumberOfNodes()!=0)
                    dimension = (mpNodes->begin())->Dimension();

                return dimension;
            }
        /// @}


        ///@}
        ///@name Nodes
        ///@{
            /// @name Access
            /// @{
            /** Returns the Node::Pointer  corresponding to it's identifier */
            typename NodeType::Pointer pGetNode(IndexType NodeId)
            {
                auto i = mpNodes->find(NodeId);
                if(i == mpNodes->end())
                {
                    std::cerr << "Node index not found: " << NodeId << "." << std::endl;
                    exit(0);
                } 
                return *i.base();
            }
            /** Returns the Node::Pointer  corresponding to it's identifier */
            const typename NodeType::Pointer pGetNode(const IndexType NodeId) const
            {
                const auto& r_nodes = *mpNodes;
                auto i = r_nodes.find(NodeId);
                if(i == r_nodes.end())
                {
                    std::cerr << "Node index not found: " << NodeId << "." << std::endl;
                    exit(0);
                } 
                return *i.base();
            }
            /** Returns a reference node corresponding to it's identifier */
            NodeType& GetNode(IndexType NodeId)
            {
                auto i = mpNodes->find(NodeId);
                if(i == mpNodes->end())
                {
                    std::cerr << "Node index not found: " << NodeId << "." << std::endl;
                    exit(0);
                } 
                return *i;
            }
            /** Returns a reference node corresponding to it's identifier */
            const NodeType& GetNode(IndexType NodeId) const
            {
                const auto& r_nodes = *mpNodes;
                auto i = r_nodes.find(NodeId);
                if(i == r_nodes.end())
                {
                    std::cerr << "Node index not found: " << NodeId << "." << std::endl;
                    exit(0);
                } 
                return *i;
            }
            
            NodeIterator NodesBegin()
            {
                return mpNodes->begin();
            }
            NodeConstantIterator NodesBegin() const
            {
                return mpNodes->begin();
            }
            NodeIterator NodesEnd()
            {
                return mpNodes->end();
            }
            NodeConstantIterator NodesEnd() const
            {
                return mpNodes->end();
            }
            NodesContainerType& Nodes()
            {
                return *mpNodes;
            }
            const NodesContainerType& Nodes() const
            {
                return *mpNodes;
            }
            typename NodesContainerType::Pointer pNodes()
            {
                return mpNodes;
            }
            typename NodesContainerType::ContainerType& NodesArray()
            {
                return mpNodes->GetContainer();
            }
            /// @}

            /// @name Inquiry
            /// @{
            SizeType NumberOfNodes() const
            {
                return mpNodes->size();
            }
            bool HasNode(IndexType NodeId) const
            {
                const auto& r_nodes = *mpNodes;
                return (r_nodes.find(NodeId) != r_nodes.end());
            }
            /// @}

            /// @name Operations
            /// @{
            /** Inserts a node in the mesh.
            */
            void AddNode(typename NodeType::Pointer pNewNode)
            {
                mpNodes->insert(mpNodes->begin(), pNewNode);
            }
            /** Remove the node with given Id from mesh.
            */
            void RemoveNode(IndexType NodeId)
            {
                mpNodes->erase(NodeId);
            }
            /** Remove given node from mesh.
            */
            void RemoveNode(NodeType& ThisNode)
            {
                mpNodes->erase(ThisNode.Id());
            }
            /** Remove given node from mesh.
            */
            void RemoveNode(typename NodeType::Pointer pThisNode)
            {
                mpNodes->erase(pThisNode->Id());
            }
            void SetNodes(typename NodesContainerType::Pointer pOtherNodes)
            {
                mpNodes = pOtherNodes;
            }
            /// @}
        /// @}

        ///@name Properties
        ///@{
            /// @name Access
            /// @{
                /** Returns the Properties::Pointer  corresponding to it's identifier */
                typename PropertiesType::Pointer pGetProperties(IndexType PropertiesId)
                {
                    return (*mpProperties)(PropertiesId);
                }
                /** Returns a reference properties corresponding to it's identifier */
                PropertiesType& GetProperties(IndexType PropertiesId)
                {
                    return (*mpProperties)[PropertiesId];
                }
                PropertiesIterator PropertiesBegin()
                {
                    return mpProperties->begin();
                }
                PropertiesConstantIterator PropertiesBegin() const
                {
                    return mpProperties->begin();
                }
                PropertiesIterator PropertiesEnd()
                {
                    return mpProperties->end();
                }
                PropertiesConstantIterator PropertiesEnd() const
                {
                    return mpProperties->end();
                }
                PropertiesContainerType& Properties()
                {
                    return *mpProperties;
                }
                const PropertiesContainerType& Properties() const
                {
                    return *mpProperties;
                }
                typename PropertiesContainerType::Pointer pProperties()
                {
                    return mpProperties;
                }
                typename PropertiesContainerType::ContainerType& PropertiesArray()
                {
                    return mpProperties->GetContainer();
                }
            /// @}
            
            /// @name Inquiry
            /// @{
                SizeType NumberOfProperties() const
                {
                    return mpProperties->size();
                }
                bool HasProperties(IndexType NodeId) const
                {
                    const auto& r_properties = *mpProperties;
                    return (r_properties.find(NodeId) != r_properties.end());
                }
            /// @}

            /// @name Operations
            /// @{
                /** Inserts a properties in the mesh.
                */
                void AddProperties(typename PropertiesType::Pointer pNewProperties)
                {
                    mpProperties->insert(mpProperties->begin(), pNewProperties);
                }
                /** Remove the properties with given Id from mesh.
                */
                void RemoveProperties(IndexType PropertiesId)
                {
                    mpProperties->erase(PropertiesId);
                }
                /** Remove given properties from mesh.
                */
                void RemoveProperties(PropertiesType& ThisProperties)
                {
                    mpProperties->erase(ThisProperties.Id());
                }
                /** Remove given properties from mesh.
                */
                void RemoveProperties(typename PropertiesType::Pointer pThisProperties)
                {
                    mpProperties->erase(pThisProperties->Id());
                }
                void SetProperties(typename PropertiesContainerType::Pointer pOtherProperties)
                {
                    mpProperties = pOtherProperties;
                }
            /// @}
        ///@}

        ///@name Elements
        ///@{
            /// @name Access
            /// @{
                /** Returns the Element::Pointer  corresponding to it's identifier */
                typename ElementType::Pointer pGetElement(IndexType ElementId)
                {
                    auto i = mpElements->find(ElementId);
                    if(i == mpElements->end())
                    {
                        std::cerr << "Element index not found: " << ElementId << "." << std::endl;
                        exit(0);
                    } 
                    return *i.base();
                }
                /** Returns the Element::Pointer  corresponding to it's identifier */
                const typename ElementType::Pointer pGetElement(const IndexType ElementId) const
                {
                    const auto& r_elements = *mpElements;
                    auto i = r_elements.find(ElementId);
                    if(i == r_elements.end())
                    {
                        std::cerr << "Element index not found: " << ElementId << "." << std::endl;
                        exit(0);
                    } 
                    return *i.base();
                }
                /** Returns a reference element corresponding to it's identifier */
                ElementType& GetElement(IndexType ElementId)
                {
                    auto i = mpElements->find(ElementId);
                    if(i == mpElements->end())
                    {
                        std::cerr << "Element index not found: " << ElementId << "." << std::endl;
                        exit(0);
                    } 
                    return *i;
                }
                /** Returns a reference element corresponding to it's identifier */
                const ElementType& GetElement(IndexType ElementId) const
                {
                    const auto& r_elements = *mpElements;
                    auto i = r_elements.find(ElementId);
                    if(i == r_elements.end())
                    {
                        std::cerr << "Element index not found: " << ElementId << "." << std::endl;
                        exit(0);
                    } 
                    return *i;
                }
                ElementIterator ElementsBegin()
                {
                    return mpElements->begin();
                }
                ElementConstantIterator ElementsBegin() const
                {
                    return mpElements->begin();
                }
                ElementIterator ElementsEnd()
                {
                    return mpElements->end();
                }
                ElementConstantIterator ElementsEnd() const
                {
                    return mpElements->end();
                }
                ElementsContainerType& Elements()
                {
                    return *mpElements;
                }
                const ElementsContainerType& Elements() const
                {
                    return *mpElements;
                }
                typename ElementsContainerType::Pointer pElements()
                {
                    return mpElements;
                }
                typename ElementsContainerType::ContainerType& ElementsArray()
                {
                    return mpElements->GetContainer();
                }

            /// @}

            /// @name Inquiry
            /// @{
                SizeType NumberOfElements() const
                {
                    return mpElements->size();
                }
                bool HasElement(IndexType ElementId) const
                {
                    const auto& r_elements = *mpElements;
                    return (r_elements.find(ElementId) != r_elements.end());
                }
            /// @}

            /// @name Operations
            /// @{
                /** Inserts a element in the mesh.
                */
                void AddElement(typename ElementType::Pointer pNewElement)
                {
                    mpElements->insert(mpElements->begin(), pNewElement);
                }
                /** Remove the element with given Id from mesh.
                */
                void RemoveElement(IndexType ElementId)
                {
                    mpElements->erase(ElementId);
                }
                /** Remove given element from mesh.
                */
                void RemoveElement(ElementType& ThisElement)
                {
                    mpElements->erase(ThisElement.Id());
                }
                /** Remove given element from mesh.
                */
                void RemoveElement(typename ElementType::Pointer pThisElement)
                {
                    mpElements->erase(pThisElement->Id());
                }
                void SetElements(typename ElementsContainerType::Pointer pOtherElements)
                {
                    mpElements = pOtherElements;
                }

            /// @}
        ///@}

        ///@name Conditions
        ///@{
            /// @name Access
            /// @{
                /** Returns the Condition::Pointer  corresponding to it's identifier */
                typename ConditionType::Pointer pGetCondition(IndexType ConditionId)
                {
                    auto i = mpConditions->find(ConditionId);
                    if(i == mpConditions->end())
                    {
                        std::cerr<< "Condition index not found: " << ConditionId << "." << std::endl;
                        exit(0);
                    } 
                    return *i.base();
                }
                /** Returns the Condition::Pointer  corresponding to it's identifier */
                const typename ConditionType::Pointer pGetCondition(const IndexType ConditionId) const
                {
                    const auto& r_conditions = *mpConditions;
                    auto i = r_conditions.find(ConditionId);
                    if(i == r_conditions.end())
                    {
                        std::cerr<< "Condition index not found: " << ConditionId << "." << std::endl;
                        exit(0);
                    } 
                    return *i.base();
                }
                /** Returns a reference condition corresponding to it's identifier */
                ConditionType& GetCondition(IndexType ConditionId)
                {
                    auto i = mpConditions->find(ConditionId);
                    if(i == mpConditions->end())
                    {
                        std::cerr<< "Condition index not found: " << ConditionId << "." << std::endl;
                        exit(0);
                    } 
                    return *i;
                }
                /** Returns a reference condition corresponding to it's identifier */
                const ConditionType& GetCondition(IndexType ConditionId) const
                {
                    const auto& r_conditions = *mpConditions;
                    auto i = r_conditions.find(ConditionId);
                    if(i == r_conditions.end())
                    {
                        std::cerr<< "Condition index not found: " << ConditionId << "." << std::endl;
                        exit(0);
                    } 
                    return *i;
                }
                ConditionIterator ConditionsBegin()
                {
                    return mpConditions->begin();
                }
                ConditionConstantIterator ConditionsBegin() const
                {
                    return mpConditions->begin();
                }
                ConditionIterator ConditionsEnd()
                {
                    return mpConditions->end();
                }
                ConditionConstantIterator ConditionsEnd() const
                {
                    return mpConditions->end();
                }
                ConditionsContainerType& Conditions()
                {
                    return *mpConditions;
                }
                const ConditionsContainerType& Conditions() const
                {
                    return *mpConditions;
                }
                typename ConditionsContainerType::Pointer pConditions()
                {
                    return mpConditions;
                }
                typename ConditionsContainerType::ContainerType& ConditionsArray()
                {
                    return mpConditions->GetContainer();
                }

            /// @}

            /// @name Inquiry
            /// @{
                SizeType NumberOfConditions() const
                {
                    return mpConditions->size();
                }
                bool HasCondition(IndexType ConditionId) const
                {
                    const auto& r_conditions = *mpConditions;
                    return (r_conditions.find(ConditionId) != r_conditions.end());
                }

            /// @}

            /// @name Operations
            /// @{
                /** Inserts a condition in the mesh.
                */
                void AddCondition(typename ConditionType::Pointer pNewCondition)
                {
                    mpConditions->insert(mpConditions->begin(), pNewCondition);
                }
                /** Remove the condition with given Id from mesh.
                */
                void RemoveCondition(IndexType ConditionId)
                {
                    mpConditions->erase(ConditionId);
                }
                /** Remove given condition from mesh.
                */
                void RemoveCondition(ConditionType& ThisCondition)
                {
                    mpConditions->erase(ThisCondition.Id());
                }
                /** Remove given condition from mesh.
                */
                void RemoveCondition(typename ConditionType::Pointer pThisCondition)
                {
                    mpConditions->erase(pThisCondition->Id());
                }
                void SetConditions(typename ConditionsContainerType::Pointer pOtherConditions)
                {
                    mpConditions = pOtherConditions;
                }

            /// @}
        ///@}
        
        /// @name Input And Output
        /// @{
            /// Turn back information as a string.
            std::string Info() const override
            {
                return "Mesh";
            }
            /// Print information about this object.
            void PrintInfo(std::ostream& rOStream) const override
            {
                rOStream << Info();
            }
            /// Print object's data.
            void PrintData(std::ostream& rOStream) const override
            {
                rOStream << "    Number of Nodes       : " << mpNodes->size() << std::endl;
                rOStream << "    Number of Properties  : " << mpProperties->size() << std::endl;
                rOStream << "    Number of Elements    : " << mpElements->size() << std::endl;
                rOStream << "    Number of Conditions  : " << mpConditions->size() << std::endl;
                rOStream << "    Number of Constraints : " << mpMasterSlaveConstraints->size() << std::endl;
            }
            /// Print information about this object.
            virtual void PrintInfo(std::ostream& rOStream, std::string const& PrefixString) const
            {
                rOStream << PrefixString << Info();
            }
            /// Print object's data.
            virtual void PrintData(std::ostream& rOStream, std::string const& PrefixString ) const
            {
                rOStream << PrefixString << "    Number of Nodes       : " << mpNodes->size() << std::endl;
                rOStream << PrefixString << "    Number of Properties  : " << mpProperties->size() << std::endl;
                rOStream << PrefixString << "    Number of Elements    : " << mpElements->size() << std::endl;
                rOStream << PrefixString << "    Number of Conditions  : " << mpConditions->size() << std::endl;
                rOStream << PrefixString << "    Number of Constraints : " << mpMasterSlaveConstraints->size() << std::endl;
            }

        /// @}
    private:
        typename NodesContainerType::Pointer mpNodes;

        typename PropertiesContainerType::Pointer mpProperties;

        typename ElementsContainerType::Pointer mpElements;

        typename ConditionsContainerType::Pointer mpConditions;

         /// Assignment operator.
        Mesh& operator=(const Mesh& rOther)
        {
            Flags::operator =(rOther);
            mpNodes = rOther.mpNodes;
            mpProperties = rOther.mpProperties;
            mpElements = rOther.mpElements;
            mpConditions = rOther.mpConditions;
            mpMasterSlaveConstraints = rOther.mpMasterSlaveConstraints;
        }
};

/// output stream function
template<class TNodeType, class TPropertiesType, class TElementType, class TConditionType>
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Mesh<TNodeType, TPropertiesType, TElementType, TConditionType>& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
#endif