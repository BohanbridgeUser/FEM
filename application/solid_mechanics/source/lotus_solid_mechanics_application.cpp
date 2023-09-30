#include "../../../include/Variable/variables.h"
#include "../../../include/Condition/condition.h"
#include "../../../include/Element/element.h"

#include "../../../include/Geometry/hexahedron.h"
#include "../../../include/Geometry/point3d.h"

#include "../include/lotus_solid_mechanics_application.h"


Lotus_Solid_Mechanics_Application::Lotus_Solid_Mechanics_Application()
:Lotus_Application("Solid_Mechanics_Application"),
mSmallDisplacementElement3D8N(0,std::make_shared<Hexahedron<Node> > (Element::GeometryType::PointsContainerType(8))),
mPointLoadCondition3D1N(0,std::make_shared<Point3D<Node> >(Element::GeometryType::PointsContainerType(1)))
{
}

void Lotus_Solid_Mechanics_Application::Register()
{
    std::stringstream banner;

    banner << "LOTUS MECHANICS\n"
           << "Initialize LotusSolidMechanicsApplication...  " << std::endl;

    LOTUS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(POINT_LOAD)

    LOTUS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(FORCE_LOAD)
    LOTUS_REGISTER_VARIABLE(FORCE_LOAD_VECTOR)

    LOTUS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MOMENT_LOAD)
    LOTUS_REGISTER_VARIABLE(MOMENT_LOAD_VECTOR)

    LOTUS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(EXTERNAL_MOMENT)

    LOTUS_REGISTER_ELEMENT("Small_Displacement_Element3D8N",mSmallDisplacementElement3D8N)

    LOTUS_REGISTER_CONDITION("Point_Load_Condition3D1N",mPointLoadCondition3D1N)

    LOTUS_REGISTER_CONSTITUTIVE_LAW("Hyper_Elastic_3D_Law",mHyperElastic3DLaw)
}
