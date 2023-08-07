#ifndef _GEOMETRY_OBJECT_H_
#define _GEOMETRY_OBJECT_H_

#include "../define.h"
#include "../node.h"
#include "../Geometry/geometry.h"
class Geometry_Object
{
    public:
        ///@name Define 
        ///@{
        LOTUS_POINTER_DEFINE(Geometry_Object)
        typedef Node NodeType;
        typedef Geometry<NodeType> GeometryType;
        ///@}

        ///@name Lift Circle 
        ///@{ 
            // Constructor
            Geometry_Object()
            {

            }
            Geometry_Object(GeometryType* ThismpGeometry):mpGeometry(ThismpGeometry)
            {

            }
            // Destructor 
            ~Geometry_Object()
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
    private:
        GeometryType* mpGeometry;
};
#endif
