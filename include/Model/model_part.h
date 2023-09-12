#ifndef _MODEL_PART_H_
#define _MODEL_PART_H_
#include "../Geometry/geometry.h"
#include "../Geometry/geometry_object.h"
#include "../Container/geometry_container.h"
#include "../Geometry/quadrilateral_3d_4.h"
#include "../Geometry/hexahedron.h"
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
        typedef Node 
                                                                        NodeType;
        typedef std::vector<NodeType> 
                                                              NodesContainerType;
        typedef Geometry<NodeType > 
                                                                    GeometryType;
        typedef Geometry_Container<GeometryType> 
                                                           GeometryContainerType;
        typedef Element 
                                                                     ElementType;
        typedef std::vector<ElementType> 
                                                           ElementsContainerType;
        typedef Condition 
                                                                   ConditionType;
        typedef std::vector<ConditionType> 
                                                         ConditionsContainerType;
        typedef Mesh<NodeType,Properties,ElementType,ConditionType> 
                                                                        MeshType;
        typedef std::vector<MeshType> 
                                                               MeshContainerType;
        typedef Dof 
                                                                         DofType;
        typedef std::vector<DofType::SharedPointer> 
                                                                  DofsVectorType;
        typedef size_t
                                                                       IndexType;
        typedef std::vector<ElementType>::iterator
                                                                 ElementIterator;
        typedef std::vector<ElementType>::const_iterator 
                                                         ElementConstantIterator;
        typedef std::vector<ConditionType>::iterator
                                                               ConditionIterator;
        typedef std::vector<ConditionType>::const_iterator 
                                                       ConditionConstantIterator;
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
        
    /// @}

    /// @name Accesse
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
        MeshType& GetMesh(IndexType index)
        {
            return mMeshContainer[index];
        }
        MeshType GetMesh(IndexType index) const
        {
            return mMeshContainer[index];
        }

        ElementIterator ElementsBegin(IndexType ThisIndex = 0)
        {
            return GetMesh(ThisIndex).ElementsBegin();
        }
        ElementConstantIterator ElementsBegin(IndexType ThisIndex = 0) const
        {
            return GetMesh(ThisIndex).ElementsBegin();
        }
        ElementIterator ElementsEnd(IndexType ThisIndex = 0)
        {
            return GetMesh(ThisIndex).ElementsEnd();
        }
        ElementConstantIterator ElementsEnd(IndexType ThisIndex = 0) const
        {
            return GetMesh(ThisIndex).ElementsEnd();
        }

        ConditionIterator ConditionsBegin(IndexType ThisIndex = 0)
        {
            return GetMesh(ThisIndex).ConditionsBegin();
        }
        ConditionConstantIterator ConditionsBegin(IndexType ThisIndex = 0) const
        {
            return GetMesh(ThisIndex).ConditionsBegin();
        }
        ConditionIterator ConditionsEnd(IndexType ThisIndex = 0)
        {
            return GetMesh(ThisIndex).ConditionsEnd();
        }
        ConditionConstantIterator ConditionsEnd(IndexType ThisIndex = 0) const
        {
            return GetMesh(ThisIndex).ConditionsEnd();
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