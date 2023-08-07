#ifndef _MESH_H_
#define _MESH_H_
#include "define.h"
#include "node.h"
#include "properties.h"
#include "Geometry/geometry.h"
#include <unordered_map>
template<typename TNodeType, typename TPropertiesType, typename TElementType, typename TConditionType>
class Mesh
{
    public:
        ///@name Define 
        ///@{
            typedef Mesh<TNodeType,TPropertiesType,TElementType,TConditionType> MeshType;
            LOTUS_POINTER_DEFINE(MeshType)

            typedef TNodeType NodeType;

            typedef TPropertiesType PropertiesType;

            typedef Geometry<NodeType> GeometryType;

            typedef TElementType ElementType;

            typedef TConditionType ConditionType;

            //typedef std::unordered_map<int,NodeType,std::hash<int>> NodesContainerType;
            typedef std::vector<NodeType> NodesContainerType;
            typedef std::shared_ptr<NodesContainerType> NodesContainerTypePointer;

            //typedef std::unordered_map<int,ElementType,std::hash<int>> ElementsContainerType;
            typedef std::vector<ElementType> ElementsContainerType;
            typedef std::shared_ptr<ElementsContainerType> ElementsContainerTypePointer;

            //typedef std::unordered_map<int,ConditionType,std::hash<int>> ConditionsContainerType;
            typedef std::vector<ConditionType> ConditionsContainerType;
            typedef std::shared_ptr<ConditionsContainerType> ConditionsContainerTypePointer;
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
    private:
        NodesContainerTypePointer mpNodes;

        Properties::SharedPointer mpProperties;

        ElementsContainerTypePointer mpElements;

        ConditionsContainerTypePointer mpConditions;
};
#endif