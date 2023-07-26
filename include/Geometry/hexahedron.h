#ifndef _HEXAHEDRON_H_
#define _HEXAHEDRON_H_
#include "geometry.h"
#include "../Container/geometry_container.h"
#include "Point.h"
template <typename DimensionType>
class Hexahedron:public Geometry<DimensionType>
{
    public:
        // @ Define { 
        LOTUS_POINTER_DEFINE(Hexahedron<DimensionType>)
        typedef Point<DimensionType::D> PointType;
        typedef std::vector<PointType> PointsVectorType;
        typedef Points_Container<PointType> PointsContainer;    
        //}

        // @ Constructor { 
        Hexahedron()
        {
            number++;
        }
        Hexahedron(PointsContainer& points):Geometry<DimensionType>(points)
        {
            number++;
            ID = number;
        }
        Hexahedron(const int& F,PointsContainer& points):Geometry<DimensionType>(points),ID(F)
        {
            number++;
        }
        Hexahedron(const Hexahedron<DimensionType>& another):Geometry<DimensionType>(another)
        {
            ID = ++number;
        }
        //}

        // @ Destructor { 
        ~Hexahedron()
        {
            if (number > 0) number--;
        }    
        //}

        // @ Utility {
        int GetID() const 
        {
            return ID;
        }
        static int GetType()
        {
            return (int)Geometry<DimensionType>::GeometryType::Hexahedron;
        }
        //}
    private:
        int ID;
        static int number;
};
template<typename DimensionType> int Hexahedron<DimensionType>::number = 0;
#endif
