#ifndef _TETRAHEDROM_H_
#define _TETRAHEDROM_H_
#include "geometry.h"
#include "../Container/geometry_container.h"
template<typename DimensionType>
class Tetrahedron:public Geometry<DimensionType>
{
    public:
        // @ Define { 
        LOTUS_POINTER_DEFINE(Tetrahedron<DimensionType>)
        typedef Point<DimensionType::D> PointType;
        typedef std::vector<PointType> PointsVectorType;
        typedef Points_Container<PointType> PointsContainer;
        //}

        // @ Constructor { 
        Tetrahedron()
        {
            number++;
        }
        Tetrahedron(PointsContainer& points):Geometry<DimensionType>(points)
        {
            number++;
            ID = number;
        }
        Tetrahedron(const int& F,PointsContainer& points):Geometry<DimensionType>(points),ID(F)
        {
            number++;
        }
        Tetrahedron(const Tetrahedron<DimensionType>& another):Geometry<DimensionType>(another)
        {
            ID = ++number;
        }
        //}

        // @ Destructor { 
        ~Tetrahedron()
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
            return (int)Geometry<DimensionType>::GeometryType::Tetrahedron;
        }
        //}
    private:
        int ID;
        static int number;
};
template<typename DimensionType> int Tetrahedron<DimensionType>::number=0;
#endif