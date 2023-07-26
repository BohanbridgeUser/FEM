#ifndef _LINE_H_
#define _LINE_H_
#include "geometry.h"
#include "Point.h"

// @ This file define class Line that consists of two point
// .1 —————— .2
template <typename DimensionType>
class Line : public Geometry<DimensionType>
{
    public:
        // @ Define {
        LOTUS_POINTER_DEFINE(Line<DimensionType>)
        typedef Point<DimensionType::D> PointType;
        typedef std::array<int,3> EdgeType;
        //}

        //@ Constructor { 
        Line():Geometry<DimensionType>()
        {
            
        }
        Line(PointType& p1, PointType& p2):Geometry<DimensionType>()
        {
            std::vector<PointType> t;
            t.push_back(p1);t.push_back(p2);
            Points_Container<PointType> p_c(t);
            Geometry<DimensionType>::pPointsVector() = p_c;
            number++;
            ID = number;
        }
        Line(Points_Container<PointType>& Points):Geometry<DimensionType>(Points)
        {
            number++;
            ID = number;
        }
        Line(const Line& another):Geometry<DimensionType>(another)
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
            return Geometry<DimensionType>::pPointsVector().GetValue(0);
        }
        PointType GetPoint1()const
        {
            return Geometry<DimensionType>::pPointsVector().GetValue(0);
        }
        PointType& GetPoint2() 
        {
            return Geometry<DimensionType>::pPointsVector().GetValue(1);
        }
        PointType GetPoint2() const
        {
            return Geometry<DimensionType>::pPointsVector().GetValue(1);
        }
        //}

        // @ Utility { 
        double Length() const
        {
            return dist(GetPoint1(),GetPoint2());
        }
        static int GetType()
        {
            return (int)Geometry<DimensionType>::GeometryType::Line;
        }    
        //}
    private:
        int ID;
        static int number;
};
template<typename DimensionType> int Line<DimensionType>:: number = 0;
#endif