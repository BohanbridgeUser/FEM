#ifndef _SHELL_ELEMENT_3D_4N_H_
#define _SHELL_ELEMENT_3D_4N_H_
#include "element.h"

class Shell_Element_3D_4N : public Element
{
    public:
        /// @name Type Define
        /// @{
            typedef Shell_Element_3D_4N 
                                                            ClassType;
            typedef Element
                                                             BaseType;
            typedef Node 
                                                             NodeType;
            typedef Geometry<NodeType> 
                                                         GeometryType;
            typedef Geometry_Data::IntegrationMethod 
                                                    IntegrationMethod;
            typedef Geometry_Data 
                                                     GeometryDataType;
                                                            
            LOTUS_POINTER_DEFINE(ClassType)

        /// @}


        /// @name Life Circle
        /// @{
            Shell_Element_3D_4N():BaseType()
            {

            }
            Shell_Element_3D_4N(GeometryType* ThismpGeometry)
            :BaseType(ThismpGeometry),
             mpProperties()
            {
                
            }
            Shell_Element_3D_4N(GeometryType* ThismpGeometry, 
                                Properties& ThisProperties)
            :BaseType(ThismpGeometry,ThisProperties)
            {
                
            }
            Shell_Element_3D_4N(ClassType& another)
            :BaseType(another)
            {

            }
            Shell_Element_3D_4N(ClassType&& another)
            :BaseType(another)
            {
                
            }
            // Destructor
            ~Element()
            {
                
            }
        /// @}

        /// @name Operators
        /// @{
            friend std::ostream& operator<<(std::ostream& os, const ClassType& another)
            {
                return os << *another.mpGeometry;
            }
        /// @}

        ///@name Operations 
        ///@{
            virtual int Check()
            {
                return 0;
            }

            virtual void Initialize(Process_Info& rCurrentProcessInfo)
            {
                
            }

            virtual void InitializeSolutionStep(Process_Info& rCurrentProcessInfo)
            {

            }

            virtual void FinalizeSolutionStep(Process_Info& rCurrentProcessInfo)
            {

            }
            /**
             * @brief This class is derived from Geometry_Object. It have methods follow:
             * GeometryType& GetGeometry() 
             * GeometryType GetGeometry() const
            */
        ///@}

        /// @name Access
        /// @{

        /// @}

        /// @name Inquiry
        /// @{

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


        /// @}


        /// @name Private Operatiors
        /// @{


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

#endif