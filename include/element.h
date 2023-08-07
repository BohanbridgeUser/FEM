#ifndef _ELEMENT_H_
#define _ELEMENT_H_
#include "Geometry/geometry_object.h"
#include "define.h"
#include "Geometry/geometry_data.h"
#include "properties.h"
class Element:public Geometry_Object
{
    public:
        ///@name Define 
        ///@{ 
        LOTUS_POINTER_DEFINE(Element)   
        typedef Element ElementType;
        typedef Geometry_Object BaseType;
        typedef Node NodeType;
        typedef Geometry<NodeType> GeometryType;
        typedef Geometry_Data::IntegrationMethod IntegrationMethod;
        typedef Geometry_Data GeometryDataType;
        ///@}

        ///@name Lift Circle 
        ///@{
            // Constructor 
            Element():Geometry_Object()
            {

            }
            Element(GeometryType* ThismpGeometry)
            :Geometry_Object(ThismpGeometry),
             mpProperties()
            {
                
            }
            Element(GeometryType* ThismpGeometry, Properties& ThisProperties)
            :Geometry_Object(ThismpGeometry),
             mpProperties(&ThisProperties)
            {
                
            }
            // Destructor
            ~Element()
            {
                
            }
        ///@}

        ///@name Operations 
        ///@{
            Properties& GetProperties()
            {
                return *mpProperties;
            }
            Properties GetProperties()const
            {
                return *mpProperties;
            }
            
            /**
             * @brief This class is derived from Geometry_Object. It have methods follow:
             * GeometryType& GetGeometry() 
             * GeometryType GetGeometry() const
            */
        ///@}
    private:
        Properties::SharedPointer mpProperties;
};
#endif