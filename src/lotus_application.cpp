#include "../include/lotus_application.h"

Lotus_Application::Lotus_Application(const std::string& ApplicationName)
: mApplicationName(ApplicationName),
  mGenericCondition( 0, GeometryType::Pointer(new Geometry<NodeType>())),
  mGenericElement( 0, GeometryType::Pointer(new Geometry<NodeType>())),
  // Components
  mpVariableData(Lotus_Components<Variable_Data>::pGetComponents()),
  mpIntVariables(Lotus_Components<Variable<int> >::pGetComponents()),
  mpUnsignedIntVariables(Lotus_Components<Variable<unsigned int> >::pGetComponents()),
  mpDoubleVariables(Lotus_Components<Variable<double> >::pGetComponents()),
  mpArray1DVariables(Lotus_Components<Variable<std::array<double, 3> > >::pGetComponents()),
  mpArray1D4Variables(Lotus_Components<Variable<std::array<double, 4> > >::pGetComponents()),
  mpArray1D6Variables(Lotus_Components<Variable<std::array<double, 6> > >::pGetComponents()),
  mpArray1D9Variables(Lotus_Components<Variable<std::array<double, 9> > >::pGetComponents()),
  mpVectorVariables(Lotus_Components<Variable<Vector> >::pGetComponents()),
  mpMatrixVariables(Lotus_Components<Variable<Matrix> >::pGetComponents()),
  mpGeometries(Lotus_Components<GeometryType>::pGetComponents()),
  mpElements(Lotus_Components<Element>::pGetComponents()),
  mpConditions(Lotus_Components<Condition>::pGetComponents())
{};

void Lotus_Application::RegisterLotusCore() 
{

    // Registering all the variables
    Lotus_Application::RegisterVariables();

    //Register specific conditions ( must be completed : conditions defined in LOTUS_application.h)
    //generic condition
    LOTUS_REGISTER_CONDITION("GenericCondition", mGenericCondition);

    //Register specific elements ( must be completed : elements defined in LOTUS_application.h)
    LOTUS_REGISTER_ELEMENT("GenericElement", mGenericElement)

    LOTUS_REGISTER_GEOMETRY("Point3D", mPoint3DPrototype);
    LOTUS_REGISTER_GEOMETRY("Hexahedron", mHexahedronPrototype);

    // Register flags:
    LOTUS_REGISTER_FLAG(STRUCTURE);
    LOTUS_REGISTER_FLAG(FLUID);
    LOTUS_REGISTER_FLAG(THERMAL);
    LOTUS_REGISTER_FLAG(VISITED);
    LOTUS_REGISTER_FLAG(SELECTED);
    LOTUS_REGISTER_FLAG(BOUNDARY);
    LOTUS_REGISTER_FLAG(INLET);
    LOTUS_REGISTER_FLAG(OUTLET);
    LOTUS_REGISTER_FLAG(SLIP);
    LOTUS_REGISTER_FLAG(INTERFACE);
    LOTUS_REGISTER_FLAG(CONTACT);
    LOTUS_REGISTER_FLAG(TO_SPLIT);
    LOTUS_REGISTER_FLAG(TO_ERASE);
    LOTUS_REGISTER_FLAG(TO_REFINE);
    LOTUS_REGISTER_FLAG(NEW_ENTITY);
    LOTUS_REGISTER_FLAG(OLD_ENTITY);
    LOTUS_REGISTER_FLAG(ACTIVE);
    LOTUS_REGISTER_FLAG(MODIFIED);
    LOTUS_REGISTER_FLAG(RIGID);
    LOTUS_REGISTER_FLAG(SOLID);
    LOTUS_REGISTER_FLAG(MPI_BOUNDARY);
    LOTUS_REGISTER_FLAG(INTERACTION);
    LOTUS_REGISTER_FLAG(ISOLATED);
    LOTUS_REGISTER_FLAG(MASTER);
    LOTUS_REGISTER_FLAG(SLAVE);
    LOTUS_REGISTER_FLAG(INSIDE);
    LOTUS_REGISTER_FLAG(FREE_SURFACE);
    LOTUS_REGISTER_FLAG(BLOCKED);
    LOTUS_REGISTER_FLAG(MARKER);
    LOTUS_REGISTER_FLAG(PERIODIC);
    LOTUS_REGISTER_FLAG(WALL);

    // Note: using internal macro for these two because they do not have a NOT_ version
    LOTUS_ADD_FLAG_TO_LOTUS_COMPONENTS(ALL_DEFINED);
    LOTUS_ADD_FLAG_TO_LOTUS_COMPONENTS(ALL_TRUE);

    // Register ConstitutiveLaw BaseClass
    LOTUS_REGISTER_CONSTITUTIVE_LAW("ConstitutiveLaw", mConstitutiveLaw);
}