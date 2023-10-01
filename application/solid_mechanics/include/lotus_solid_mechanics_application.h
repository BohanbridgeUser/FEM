#ifndef _LOTUS_SOLID_MECHANICS_APPLICATION_
#define _LOTUS_SOLID_MECHANICS_APPLICATION_

#include "../../../include/define.h"
#include "../../../include/lotus_application.h"

#include "lotus_solid_mechanics_variables.h"

/*Elements*/
#include "solid_mechanics_element/small_displacement_element.h"

/*Conditions*/
#include "solid_mechanics_condition/point_load_condition.h"

/**Constitutive Law**/
#include "solid_mechanics_constitutive_law/hyperelastic_3D_law.h"
#include "solid_mechanics_constitutive_law/linear_elastic_3D_law.h"

class Lotus_Solid_Mechanics_Application : public Lotus_Application
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_SHARED_POINTER_DEFINE(Lotus_Solid_Mechanics_Application)
            typedef Lotus_Application
                                                                        BaseType;
            typedef typename BaseType::NodeType
                                                                        NodeType;
            typedef typename BaseType::GeometryType
                                                                    GeometryType;
            typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                                          Vector;
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                          Matrix;

        /// @}


        /// @name Life Circle
        /// @{
            Lotus_Solid_Mechanics_Application();

            ~Lotus_Solid_Mechanics_Application() override {}

        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            void Register() override;

        /// @}


        /// @name Access
        /// @{


        /// @}


        /// @name Inquiry
        /// @{


        /// @}

        /// @name Input And Output
        /// @{
            std::string Info() const override
            {
                return "Lotus Solid Mechanics Application";
            }

            /// Print information about this object.
            void PrintInfo(std::ostream& rOStream) const override
            {
                rOStream << Info();
                PrintData(rOStream);
            }

            ///// Print object's data.
            void PrintData(std::ostream& rOStream) const override
            {
                std::cout << "\"in LotusSolidMechanicsApplication\"" << " : " << "in LotusSolidMechanicsApplication" << std::endl;
                std::cout << "KratosComponents<VariableData>::GetComponents().size()" 
                            << " : " 
                            << Lotus_Components<Variable_Data>::GetComponents().size() << std::endl;
                rOStream << "Variables:" << std::endl;
                Lotus_Components<Variable_Data>().PrintData(rOStream);
                rOStream << std::endl;
                rOStream << "Elements:" << std::endl;
                Lotus_Components<Element>().PrintData(rOStream);
                rOStream << std::endl;
                rOStream << "Conditions:" << std::endl;
                Lotus_Components<Condition>().PrintData(rOStream);
            }
        /// @}
    protected:
        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{


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
            //elements
            const Small_Displacement_Element mSmallDisplacementElement3D8N;

            //conditions
            const Point_Load_Condition       mPointLoadCondition3D1N;

            //constitutive law
            const Hyper_Elastic_3D_Law       mHyperElastic3DLaw;

            const Linear_Elastic_3D_Law      mLinearElastic3DLaw;
        /// @}


        /// @name Private Operatiors
        /// @{
            Lotus_Solid_Mechanics_Application& operator=(Lotus_Solid_Mechanics_Application const& rOther);

        /// @}


        /// @name Private Operations
        /// @{
            /// Assignment operator.
            

            /// Copy constructor.
            Lotus_Solid_Mechanics_Application(Lotus_Solid_Mechanics_Application const& rOther);

        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};

#endif