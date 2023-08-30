#ifndef _HEXAHEDRON_H_
#define _HEXAHEDRON_H_
#include "geometry.h"
#include "../Container/geometry_container.h"
#include "Point.h"
template <typename TPointType>
class Hexahedron:public Geometry<TPointType>
{
    public:
        // @ Define { 
        LOTUS_POINTER_DEFINE(Hexahedron<TPointType>)
        typedef TPointType 
                                                            PointType;
        typedef std::vector<PointType> 
                                                     PointsVectorType;
        typedef std::vector<PointType> 
                                                      PointsContainer;    
        //}

        // @ Constructor { 
        Hexahedron()
        {
            number++;
        }
        Hexahedron(PointsContainer& points)
        :Geometry<TPointType>(points)
        {
            number++;
            ID = number;
        }
        Hexahedron(const int& F,PointsContainer& points)
        :Geometry<TPointType>(points),ID(F)
        {
            number++;
        }
        Hexahedron(const Hexahedron<TPointType>& another)
        :Geometry<TPointType>(another)
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
            return (int)Geometry<TPointType>::Geometry_Type::Hexahedron;
        }
        //}
    private:
        int ID;
        static int number;
};
template<typename TPointType> int Hexahedron<TPointType>::number = 0;
#endif
