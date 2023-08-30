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
template<typename TPointType>
class Triangle:public Geometry<TPointType>
{
    public:
        // @ Define { 
        LOTUS_POINTER_DEFINE(Triangle<TPointType>)  
        typedef TPointType 
                                                    PointType;
        typedef std::vector<PointType> 
                                             PointsVectorType;
        //}

        // @ Constructor { 
        Triangle():Geometry<TPointType>()
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
            Points_Container<TPointType> t_c(t);
            Geometry<TPointType>::pPointsVector() = t_c;
        }
        Triangle(PointsVectorType& points)
        :Geometry<TPointType>(points)
        {
            number++;
            ID = number;
        }
        Triangle(const int& F,PointsVectorType& points)
        :Geometry<TPointType>(points),
         ID(F)
        {
            number++;
        }
        Triangle(const Triangle<TPointType>& another)
        :Geometry<TPointType>(another)
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
            return (int)Geometry<TPointType>::Geometry_Type::Triangle;
        }
        //}
    private:
        int ID;
        static int number;
};

template<typename TPointType> int Triangle<TPointType>::number = 0;
#endif
