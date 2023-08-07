#ifndef _MODEL_PART_H_
#define _MODEL_PART_H_
#include "../define.h"
#include "../Geometry/geometry.h"
#include "../Geometry/geometry_object.h"
#include "../Container/geometry_container.h"
#include "../Geometry/Point.h"
#include "../node.h"
#include "../element.h"
#include "../condition.h"
#include "../properties.h"
#include "../Process/process.h"
#include "../mesh.h"
#include "../process_info.h"
#include "model.h"
#include <string>
#include <vector>
class Model;
class Model_Part
{
public:
    /// @name Define
    /// @{ 
    LOTUS_POINTER_DEFINE(Model_Part);
    typedef Node NodeType;
    typedef std::vector<NodeType> NodesContainerType;

    typedef Geometry<NodeType > GeometryType;
    typedef Geometry_Container<GeometryType> GeometryContainerType;

    typedef Element ElementType;
    typedef std::vector<ElementType> ElementsContainerType;

    typedef Condition ConditionType;
    typedef std::vector<ConditionType> ConditionsContainerType;

    typedef Mesh<NodeType,Properties,ElementType,ConditionType> MeshType;
    typedef std::vector<MeshType> MeshContainerType;

    typedef Dof DofType;
    typedef std::vector<DofType::SharedPointer> DofsVectorType;
    ///@} 

    ///@name Life Circle
    ///@{
        // Constructor
        Model_Part(Model& OwnerModel):mrModel(OwnerModel)
        {
            
        }
        Model_Part(std::string name,Model& OwnerModel):mName(name),mrModel(OwnerModel)
        {
            
        }

        // Destructor
        ~Model_Part()
        {

        }
    ///@}  


private:
    friend class Model;
    std::string mName;
    GeometryContainerType mGeometryContainer;
    MeshContainerType mMeshContainer;
    Process_info::SharedPointer mpProcessinfoPointer;
    Model_Part* mpParentModelPart = NULL;
    Model& mrModel;
};
#endif