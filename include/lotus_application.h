#pragma once

/**
 * @brief system includes
*/
#include <string>
#include <iostream>

/**
 * @brief project includes
*/
#include "define.h"
#include "Element/element.h"
#include "Condition/condition.h"
#include "lotus_components.h"
#include "Geometry/geometry_object.h"

/**
 * @brief Geometry
*/
#include "Geometry/register_lotus_components_for_geometry.h"
#include "Geometry/hexahedron.h"
#include "Geometry/point3d.h"

/**
 * @brief Element
*/
#include "Element/element.h"
#include "Element/mesh_element.h"

/**
 * @brief Condition
*/
#include "Condition/condition.h"
#include "Condition/mesh_condition.h"


class Lotus_Application
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_SHARED_POINTER_DEFINE(Lotus_Application)
            typedef Node
                                                                        NodeType;
            typedef Geometry<NodeType>
                                                                    GeometryType;
            typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                                          Vector;
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                          Matrix;
        /// @}


        /// @name Life Circle
        /// @{
            /// Default constructor.
            explicit Lotus_Application(const std::string& ApplicationName);

            Lotus_Application() = delete;

            /// Copy constructor.
            Lotus_Application(Lotus_Application const& rOther)
                :mpVariableData(rOther.mpVariableData),
                mpIntVariables(rOther.mpIntVariables),
                mpUnsignedIntVariables(rOther.mpUnsignedIntVariables),
                mpDoubleVariables(rOther.mpDoubleVariables),
                mpArray1DVariables(rOther.mpArray1DVariables),
                mpArray1D4Variables(rOther.mpArray1D4Variables),
                mpArray1D6Variables(rOther.mpArray1D6Variables),
                mpArray1D9Variables(rOther.mpArray1D9Variables),
                mpVectorVariables(rOther.mpVectorVariables),
                mpMatrixVariables(rOther.mpMatrixVariables),
                mpGeometries(rOther.mpGeometries),
                mpElements(rOther.mpElements),
                mpConditions(rOther.mpConditions)
                {
                }

            /// Destructor.
            virtual ~Lotus_Application() {}

        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            virtual void Register()
            {
                RegisterLotusCore();
            }

            void RegisterLotusCore();

            ///////////////////////////////////////////////////////////////////
            void RegisterVariables();  // This contains the whole list of common variables in the Kratos Core

            void SetComponents(Lotus_Components<Variable_Data>::ComponentsContainerType const& VariableDataComponents)
            {
                for (auto it = mpVariableData->begin(); it != mpVariableData->end(); it++) {
                    std::string const& r_variable_name = it->second->Name();
                    auto it_variable = VariableDataComponents.find(r_variable_name);
                    if(it_variable == VariableDataComponents.end())
                    {
                        std::cerr<< "This variable is not registered in Kernel : " << *(it_variable->second) << std::endl;
                        exit(0);
                    } 
                }
            }
            void SetComponents(Lotus_Components<Geometry<Node>>::ComponentsContainerType const& GeometryComponents)
            {
                mpGeometries->insert(GeometryComponents.begin(), GeometryComponents.end());
            }
            void SetComponents(Lotus_Components<Element>::ComponentsContainerType const&ElementComponents)
            {
                // It's better to make a loop over new components and add them if they are NOT already exist in application. Or make an ERROR for incompatibility between applications.
                mpElements->insert(ElementComponents.begin(), ElementComponents.end());
            }
            void SetComponents(Lotus_Components<Condition>::ComponentsContainerType const& ConditionComponents)
            {
                mpConditions->insert(
                    ConditionComponents.begin(), ConditionComponents.end());
            }

        /// @}


        /// @name Access
        /// @{
            // I have to see why the above version is not working for multi thread ...
            // Anyway its working with these functions.Pooyan.
            Lotus_Components<Variable<int> >::ComponentsContainerType& 
            GetComponents(Variable<int> const& rComponentType) 
            {
                return *mpIntVariables;
            }
            Lotus_Components<Variable<unsigned int> >::ComponentsContainerType&
            GetComponents(Variable<unsigned int> const& rComponentType) 
            {
                return *mpUnsignedIntVariables;
            }
            Lotus_Components<Variable<double> >::ComponentsContainerType& 
            GetComponents(Variable<double> const& rComponentType) 
            {
                return *mpDoubleVariables;
            }
            Lotus_Components<Variable<std::array<double, 3> > >::ComponentsContainerType&
            GetComponents(Variable<std::array<double, 3> > const& rComponentType) 
            {
                return *mpArray1DVariables;
            }
            Lotus_Components<Variable<std::array<double, 4> > >::ComponentsContainerType&
            GetComponents(Variable<std::array<double, 4> > const& rComponentType) 
            {
                return *mpArray1D4Variables;
            }
            Lotus_Components<Variable<std::array<double, 6> > >::ComponentsContainerType&
            GetComponents(Variable<std::array<double, 6> > const& rComponentType) 
            {
                return *mpArray1D6Variables;
            }
            Lotus_Components<Variable<std::array<double, 9> > >::ComponentsContainerType&
            GetComponents(Variable<std::array<double, 9> > const& rComponentType) 
            {
                return *mpArray1D9Variables;
            }
            Lotus_Components<Variable<Vector> >::ComponentsContainerType& GetComponents(
                Variable<Vector> const& rComponentType) 
            {
                return *mpVectorVariables;
            }
            Lotus_Components<Variable<Matrix> >::ComponentsContainerType& GetComponents(
                Variable<Matrix> const& rComponentType)
            {
                return *mpMatrixVariables;
            }
            Lotus_Components<Variable_Data>::ComponentsContainerType& GetVariables() 
            {
                return *mpVariableData;
            }
            Lotus_Components<Geometry<Node>>::ComponentsContainerType& GetGeometries() 
            {
                return *mpGeometries;
            }
            Lotus_Components<Element>::ComponentsContainerType& GetElements() 
            {
                return *mpElements;
            }
            Lotus_Components<Condition>::ComponentsContainerType& GetConditions() 
            {
                return *mpConditions;
            }

        /// @}


        /// @name Inquiry
        /// @{
            const std::string& Name() const { return mApplicationName; }

        /// @}

        /// @name Input And Output
        /// @{
            /// Turn back information as a string.
            virtual std::string Info() const
            {
                return "LotusApplication";
            }
            /// Print information about this object.
            virtual void PrintInfo(std::ostream& rOStream) const

            {
                rOStream << Info();
            }
            /// Print object's data.

            virtual void PrintData(std::ostream& rOStream) const
            {
                rOStream << "Variables:" << std::endl;

                Lotus_Components<Variable_Data>().PrintData(rOStream);

                rOStream << std::endl;

                rOStream << "Geometries:" << std::endl;

                Lotus_Components<Geometry<Node>>().PrintData(rOStream);

                rOStream << "Elements:" << std::endl;

                Lotus_Components<Element>().PrintData(rOStream);

                rOStream << std::endl;

                rOStream << "Conditions:" << std::endl;

                Lotus_Components<Condition>().PrintData(rOStream);

                rOStream << std::endl;
            }

        /// @}
    protected:
        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{
            std::string mApplicationName;

            const Point<3> mPointPrototype;
            const Point3D<NodeType> mPoint3DPrototype = Point3D<NodeType>(GeometryType::PointsContainerType(1));

            const Hexahedron<NodeType> mHexahedronPrototype = Hexahedron<NodeType>(GeometryType::PointsContainerType(8));

            // Element
            const MeshElement mGenericElement;

            // Condition
            const MeshCondition mGenericCondition;

            // Base constitutive law definition
            const Constitutive_Law mConstitutiveLaw;

            // Lotus_Components definition
            Lotus_Components<Variable_Data>::ComponentsContainerType* mpVariableData;

            Lotus_Components<Variable<int> >::ComponentsContainerType* mpIntVariables;

            Lotus_Components<Variable<unsigned int> >::ComponentsContainerType* mpUnsignedIntVariables;

            Lotus_Components<Variable<double> >::ComponentsContainerType* mpDoubleVariables;

            Lotus_Components<Variable<std::array<double, 3> > >::ComponentsContainerType* mpArray1DVariables;

            Lotus_Components<Variable<std::array<double, 4> > >::ComponentsContainerType* mpArray1D4Variables;

            Lotus_Components<Variable<std::array<double, 6> > >::ComponentsContainerType* mpArray1D6Variables;

            Lotus_Components<Variable<std::array<double, 9> > >::ComponentsContainerType* mpArray1D9Variables;

            Lotus_Components<Variable<Vector> >::ComponentsContainerType* mpVectorVariables;

            Lotus_Components<Variable<Matrix> >::ComponentsContainerType* mpMatrixVariables;

            Lotus_Components<Geometry<Node>>::ComponentsContainerType* mpGeometries;

            Lotus_Components<Element>::ComponentsContainerType* mpElements;

            Lotus_Components<Condition>::ComponentsContainerType* mpConditions;

        /// @}


        /// @name Protected Operatiors
        /// @{


        /// @}


        /// @name Protected Operations
        /// @{


        /// @}


        /// @name Protected Access
        /// @{


        /// @}


        /// @name Protected Inquiry
        /// @{


        /// @}


    private:
        /// @name Private Static Member Variables
        /// @{


        /// @}


        /// @name Private Member Variables
        /// @{


        /// @}


        /// @name Private Operatiors
        /// @{
            Lotus_Application& operator=(Lotus_Application const& rOther);

        /// @}


        /// @name Private Operations
        /// @{


        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};
/// output stream function

inline std::ostream& operator<<(std::ostream& rOStream,
    const Lotus_Application& rThis)
{
    rThis.PrintInfo(rOStream);

    rOStream << std::endl;

    rThis.PrintData(rOStream);

    return rOStream;
}
