#ifndef _LINE_H_
#define _LINE_H_
#include "geometry.h"
#include "Point.h"

// @ This file define class Line that consists of two point
// .1 —————— .2
template <typename TPointType>
class Line : public Geometry<TPointType>
{
    public:
        // @ Define {
        LOTUS_POINTER_DEFINE(Line<TPointType>)
        typedef TPointType PointType;
        typedef std::array<int,3> EdgeType;
        //}

        //@ Constructor { 
        Line():Geometry<PointType>()
        {
            
        }
        Line(PointType& p1, PointType& p2):Geometry<PointType>()
        {
            std::vector<PointType> t;
            t.push_back(p1);t.push_back(p2);
            Points_Container<PointType> p_c(t);
            Geometry<PointType>::pPointsVector() = p_c;
            number++;
            ID = number;
        }
        Line(Points_Container<PointType>& Points):Geometry<PointType>(Points)
        {
            number++;
            ID = number;
        }
        Line(const Line& another):Geometry<PointType>(another)
        {
            number++;
            ID = number;
        }
        //}

        // @ Destructor {  
        ~Line()
        {
            if (number > 0 ) number--;
        }
        //}

        // @ Operations { 
        int GetID()const
        {
            return ID;
        }
        PointType& GetPoint1()
        {
            return Geometry<PointType>::pPointsVector().GetValue(0);
        }
        PointType GetPoint1()const
        {
            return Geometry<PointType>::pPointsVector().GetValue(0);
        }
        PointType& GetPoint2() 
        {
            return Geometry<PointType>::pPointsVector().GetValue(1);
        }
        PointType GetPoint2() const
        {
            return Geometry<PointType>::pPointsVector().GetValue(1);
        }
        //}

        // @ Utility { 
        double Length() const
        {
            return dist(GetPoint1(),GetPoint2());
        }
        static int GetType()
        {
            return (int)Geometry<PointType>::Geometry_Type::Line;
        }    
        //}
    private:
        int ID;
        static int number;
};
template<typename PointType> int Line<PointType>:: number = 0;
#endif