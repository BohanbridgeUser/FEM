#ifndef _CONDITION_H_
#define _CONDITION_H_
#include "define.h"
#include "Geometry/geometry.h"
#include "Geometry/geometry_data.h"
#include "Geometry/geometry_object.h"
#include "properties.h"
class Condition : public Geometry_Object
{
    public: 
        ///@name Define
        ///@{
        LOTUS_POINTER_DEFINE(Condition)
        typedef Condition ConditionType;
        typedef Geometry_Object BaseType;
        typedef Node NodeType;
        typedef Geometry<NodeType> GeometryType;
        typedef Geometry_Data::IntegrationMethod IntegrationMethod;
        typedef Geometry_Data GeometryDataType; 
        ///@}

        ///@name Life Circle
        ///@{
            // Constructor
            Condition():Geometry_Object()
            {

            }
            Condition(GeometryType* mpThisGeometry, Properties& ThisProperties)
            :Geometry_Object(mpThisGeometry),
             mpProperties(&ThisProperties)
            {

            }
            // Destructor
            ~Condition()
            {

            }
        ///@}
    private:
        Properties::SharedPointer mpProperties;
};

#endif