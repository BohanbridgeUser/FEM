#ifndef _TETRAHEDROM_H_
#define _TETRAHEDROM_H_
#include "geometry.h"
#include "../Container/geometry_container.h"
template<typename TPointType>
class Tetrahedron:public Geometry<TPointType>
{
    public:
        // @ Define { 
        LOTUS_POINTER_DEFINE(Tetrahedron<TPointType>)
        typedef TPointType 
                                                        PointType;
        typedef std::vector<PointType> 
                                                 PointsVectorType;
        typedef std::vector<PointType> 
                                                  PointsContainer;
        //}

        // @ Constructor { 
        Tetrahedron()
        {
            number++;
        }
        Tetrahedron(PointsContainer& points)
        :Geometry<TPointType>(points)
        {
            number++;
            ID = number;
        }
        Tetrahedron(const int& F,PointsContainer& points)
        :Geometry<TPointType>(points),ID(F)
        {
            number++;
        }
        Tetrahedron(const Tetrahedron<TPointType>& another)
        :Geometry<TPointType>(another)
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
            return (int)Geometry<TPointType>::Geometry_Type::Tetrahedron;
        }
        //}
    private:
        int ID;
        static int number;
};
template<typename TPointType> int Tetrahedron<TPointType>::number=0;
#endif