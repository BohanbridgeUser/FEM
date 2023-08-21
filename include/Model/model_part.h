#ifndef _MODEL_PART_H_
#define _MODEL_PART_H_
#include "../Geometry/geometry.h"
#include "../Geometry/geometry_object.h"
#include "../Container/geometry_container.h"
#include "../Geometry/quadrilateral_3d_4.h"
#include "../Node/node.h"
#include "../Element/element.h"
#include "../Condition/condition.h"
#include "../Property/properties.h"
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

    /// @name Operators
    /// @{

    /// @}

    /// @name Operations
    /// @{
        ElementsContainerType& Elements(int Index = 0)
        {
            return mMeshContainer[Index].Elements();
        }

        ConditionsContainerType& Conditions(int Index = 0)
        {
            return mMeshContainer[Index].Conditions();
        }
        Process_Info& GetProcessInfo()
        {
            return *mpProcessinfoPointer;
        }
    /// @}

private:
    friend class Model;
    std::string mName;
    GeometryContainerType mGeometryContainer;
    MeshContainerType mMeshContainer;   
    Process_Info::SharedPointer mpProcessinfoPointer;
    Model_Part* mpParentModelPart = NULL;
    Model& mrModel;
};
#endif