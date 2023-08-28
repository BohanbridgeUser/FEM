#ifndef _MESH_H_
#define _MESH_H_
#include "define.h"
#include "Node/node.h"
#include "Property/properties.h"
#include "Geometry/geometry.h"
#include "Element/element.h"
#include <unordered_map>
#include <vector>
template<typename TNodeType, typename TPropertiesType, typename TElementType, typename TConditionType>
class Mesh
{
    public:
        ///@name Define 
        ///@{
            typedef Mesh<TNodeType,TPropertiesType,TElementType,TConditionType> MeshType;
            LOTUS_POINTER_DEFINE(MeshType)

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
            typedef size_t
                                                                       IndexType;
            //typedef std::unordered_map<int,NodeType,std::hash<int>> NodesContainerType;
            typedef std::vector<NodeType> 
                                                              NodesContainerType;
            typedef std::shared_ptr<NodesContainerType> 
                                                       NodesContainerTypePointer;

            //typedef std::unordered_map<int,ElementType,std::hash<int>> ElementsContainerType;
            typedef std::vector<ElementType> 
                                                           ElementsContainerType;
            typedef std::shared_ptr<ElementsContainerType> 
                                                    ElementsContainerTypePointer;
            typedef typename std::vector<ElementType>::iterator
                                                                 ElementIterator;
            typedef typename std::vector<ElementType>::const_iterator 
                                                         ElementConstantIterator;
            //typedef std::unordered_map<int,ConditionType,std::hash<int>> ConditionsContainerType;
            typedef std::vector<ConditionType> 
                                                         ConditionsContainerType;
            typedef std::shared_ptr<ConditionsContainerType> 
                                                  ConditionsContainerTypePointer;
            typedef typename std::vector<ConditionType>::iterator
                                                               ConditionIterator;
            typedef typename std::vector<ConditionType>::const_iterator 
                                                       ConditionConstantIterator;
        ///@}

        ///@name Life Circle
        ///@{
            // Constructor
            Mesh()
            {

            }
            Mesh(NodesContainerType* ThisNodes, Properties& ThisProperties, 
                 ElementsContainerType& ThisElements, ConditionsContainerType& ThisConditions)
                 :mpNodes(ThisNodes),
                  mpProperties(&ThisProperties),
                  mpElements(&ThisElements),
                  mpConditions(&ThisConditions)
            {


            }
            // Destructor
            ~Mesh()
            {

            }
        ///@}

        /// @name Operators
        /// @{

        /// @}

        /// @name Operations
        /// @{
            ElementsContainerType& Elements()
            {
                return *mpElements;
            }
            ConditionsContainerType& Conditions()
            {
                return *mpConditions;
            }
            PropertiesType& Property()
            {
                return *mpProperties;
            }
            NodesContainerType& Nodes()
            {
                return *mpNodes;
            }
        /// @}

        /// @name Access
        /// @{
            ElementIterator ElementsBegin(IndexType ThisIndex = 0)
            {
                return mpElements->begin();
            }
            ElementConstantIterator ElementsBegin(IndexType ThisIndex = 0) const
            {
                return mpElements->cbegin();
            }
            ElementIterator ElementsEnd(IndexType ThisIndex = 0)
            {
                return mpElements->end();
            }
            ElementConstantIterator ElementsEnd(IndexType ThisIndex = 0) const
            {
                return mpElements->cend();
            }

            ConditionIterator ConditionsBegin(IndexType ThisIndex = 0)
            {
                return mpConditions->begin();
            }
            ConditionConstantIterator ConditionsBegin(IndexType ThisIndex = 0) const
            {
                return mpConditions->cbegin();
            }
            ConditionIterator ConditionsEnd(IndexType ThisIndex = 0)
            {
                return mpConditions->end();
            }
            ConditionConstantIterator ConditionsEnd(IndexType ThisIndex = 0) const
            {
                return mpConditions->cend();
            }
        /// @}
    private:
        NodesContainerType* mpNodes;

        Properties* mpProperties;

        ElementsContainerType* mpElements;

        ConditionsContainerType* mpConditions;
};
#endif