#ifndef _QUADRANGLE_H_
#define _QUADRANGLE_H_
#include "geometry.h"
#include "../Container/geometry_container.h"
#include "Point.h"
template <typename DimensionType>
class Quadrangle:public Geometry<DimensionType>
{
    public:
        // @ Define { 
        LOTUS_POINTER_DEFINE(Quadrangle<DimensionType>)
        typedef Point<DimensionType::D> PointType;
        typedef std::vector<PointType> PointsVectorType;
        typedef Points_Container<PointType> PointsContainer;
        //}

        // @ Constructor { 
        Quadrangle()
        {
            number++;
        }
        Quadrangle(PointsContainer& points):Geometry<DimensionType>(points)
        {
            number++;
            ID = number;
        }
        Quadrangle(const int& F,PointsContainer& points):Geometry<DimensionType>(points),ID(F)
        {
            number++;
        }
        Quadrangle(const Quadrangle<DimensionType>& another):Geometry<DimensionType>(another)
        {
            ID = ++number;
        }
        //}

        // @ Destructor { 
        ~Quadrangle()
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
            return (int)Geometry<DimensionType>::GeometryType::Quadrangle;
        }
        //}
    private:
        int ID;
        static int number;
};
template <typename DimensionType> int Quadrangle<DimensionType>::number = 0;
#endif