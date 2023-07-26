#ifndef _Triangle_H_
#define _Triangle_H_
#include "geometry.h"
#include "../Container/geometry_container.h"
#include "line.h"
// @ This file define template class Triangle
//   ths sequence of edges in this class observe right-hand rule.
//      
//      
//      .2
//      |  `\
//      |    `\
//      |      `\
//      |        `\  
//      .0 - - - - .1
template<typename DimensionType>
class Triangle:public Geometry<DimensionType>
{
    public:
        // @ Define { 
        LOTUS_POINTER_DEFINE(Triangle<DimensionType>)  
        typedef Point<DimensionType::D> PointType;
        typedef std::vector<PointType> PointsVectorType;
        typedef Points_Container<PointType> PointsContainer;
        //}

        // @ Constructor { 
        Triangle():Geometry<DimensionType>()
        {
            number++;
        }
        Triangle(const PointType& p1, const PointType& p2, const PointType& p3)
        {
            number++;
            ID = number;
            std::vector<PointType> t;
            t.push_back(p1);
            t.push_back(p2);
            t.push_back(p3);
            Points_Container<DimensionType> t_c(t);
            Geometry<DimensionType>::pPointsVector() = t_c;
        }
        Triangle(PointsContainer& points):Geometry<DimensionType>(points)
        {
            number++;
            ID = number;
        }
        Triangle(const int& F,PointsContainer& points):Geometry<DimensionType>(points),ID(F)
        {
            number++;
        }
        Triangle(const Triangle<DimensionType>& another):Geometry<DimensionType>(another)
        {
            ID = ++number;
        }
        //}

        // @ Destructor { 
        ~Triangle()
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
            return (int)Geometry<DimensionType>::GeometryType::Triangle;
        }
        //}
    private:
        int ID;
        static int number;
};

template<typename DimensionType> int Triangle<DimensionType>::number = 0;
#endif
