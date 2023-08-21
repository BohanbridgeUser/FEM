#ifndef _GEOMETRY_OBJECT_H_
#define _GEOMETRY_OBJECT_H_

#include "../define.h"
#include "../Node/node.h"
#include "../Geometry/geometry.h"
class Geometry_Object
{
    public:
        ///@name Define 
        ///@{
            LOTUS_POINTER_DEFINE(Geometry_Object)
            typedef Node NodeType;
            typedef Geometry<NodeType> GeometryType;
            typedef Geometry_Object ClassType;
        ///@}

        ///@name Lift Circle 
        ///@{ 
            // Constructor
            Geometry_Object()
            {
                mpGeometry = nullptr;
            }
            Geometry_Object(GeometryType* ThismpGeometry)
            :mpGeometry(ThismpGeometry)
            {

            }
            Geometry_Object(ClassType& another)
            :mpGeometry(another.mpGeometry)
            {

            }
            Geometry_Object(ClassType&& another)
            :mpGeometry(another.mpGeometry)
            {

            }
            // Destructor 
            virtual ~Geometry_Object()
            {

            }
        ///@}

        ///@name Operations 
        ///@{
            GeometryType& GetGeometry()
            {
                return *mpGeometry;
            }
            GeometryType GetGeometry() const
            {
                return *mpGeometry;
            }
        ///@}
    protected:
        GeometryType* mpGeometry;
};
#endif
