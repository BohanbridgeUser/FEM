#include "../../include/Condition/mesh_condition.h"


MeshCondition::MeshCondition(IndexType NewId)
:BaseType(NewId)
{
}

/***********************************************************************************/
/***********************************************************************************/

MeshCondition::MeshCondition(
    IndexType NewId,
    const NodesArrayType& rThisNodes
    ) : BaseType(NewId, rThisNodes)
{
}

/***********************************************************************************/
/***********************************************************************************/

MeshCondition::MeshCondition(
    IndexType NewId,
    GeometryType::Pointer pGeometry
    ) : BaseType(NewId, pGeometry)
{
}

/***********************************************************************************/
/***********************************************************************************/

MeshCondition::MeshCondition(
    IndexType NewId,
    GeometryType::Pointer pGeometry,
    PropertiesType::Pointer pProperties
    ) : BaseType(NewId,pGeometry, pProperties)
{
}

/***********************************************************************************/
/***********************************************************************************/

MeshCondition::MeshCondition(MeshCondition const& rOther)
    : BaseType(rOther)
{
}

/***********************************************************************************/
/***********************************************************************************/

MeshCondition::~MeshCondition()
{
}

/***********************************************************************************/
/***********************************************************************************/

MeshCondition& MeshCondition::operator=(MeshCondition const& rOther)
{
    //ALL MEMBER VARIABLES THAT MUST BE KEPT IN AN "=" OPERATION NEEDS TO BE COPIED HERE

    Condition::operator=(rOther);

    return *this;
}

/***********************************************************************************/
/***********************************************************************************/

Condition::Pointer MeshCondition::Create(
    IndexType NewId,
    NodesArrayType const& ThisNodes,
    PropertiesType::Pointer pProperties
    ) const
{
    return std::make_shared<MeshCondition>(NewId, GetGeometry().Create(ThisNodes), pProperties);
}

/***********************************************************************************/
/***********************************************************************************/

Condition::Pointer MeshCondition::Create(
    IndexType NewId,
    GeometryType::Pointer pGeom,
    PropertiesType::Pointer pProperties
    ) const
{
    
    return std::make_shared<MeshCondition>(NewId, pGeom, pProperties);
    
}

/***********************************************************************************/
/***********************************************************************************/

Condition::Pointer MeshCondition::Clone (
    IndexType NewId,
    NodesArrayType const& ThisNodes
    ) const
{
    

    Condition::Pointer p_new_cond = std::make_shared<MeshCondition>(NewId, GetGeometry().Create(ThisNodes), pGetProperties());
    p_new_cond->SetData(this->GetData());
    p_new_cond->Set(Flags(*this));
    return p_new_cond;

    
}

/***********************************************************************************/
/***********************************************************************************/

void MeshCondition::AddExplicitContribution(
    const VectorType& rRHSVector,
    const Variable<VectorType>& rRHSVariable,
    const Variable<double >& rDestinationVariable,
    const Process_Info& rCurrentProcessInfo
    )
{
}

/***********************************************************************************/
/***********************************************************************************/

void MeshCondition::AddExplicitContribution(
    const VectorType& rRHS,
    const Variable<VectorType>& rRHSVariable,
    const Variable<std::array<double,3> >& rDestinationVariable,
    const Process_Info& rCurrentProcessInfo
    )
{
}

/***********************************************************************************/
/***********************************************************************************/

void MeshCondition::AddExplicitContribution(
    const MatrixType& rLHSMatrix,
    const Variable<MatrixType>& rLHSVariable,
    const Variable<Matrix>& rDestinationVariable,
    const Process_Info& rCurrentProcessInfo
    )
{
}

