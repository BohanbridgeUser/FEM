#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_
#include "../define.h"
#include "geometry_dimension.h"
#include "../Container/points_container.h"
#include "geometry_data.h"
#include <vector>
#include <array>
#include <memory>
template<class DimensionType>
class Geometry 
{
    public:
        enum class GeometryType {
            Line,
            Triangle,
            Quadrangle,
            Tetrahedron,
            Hexahedron
        };

        LOTUS_POINTER_DEFINE(Geometry)

        using PointType = Point<DimensionType::D>;
        //using Geometry_Data = Geometry_Data<DimensionType>;
        // @ Constructor { 
            Geometry()
            {
               std::vector<PointType> pPoints;
            }    
            Geometry(Points_Container<PointType>& Points):pPoints(Points)
            {

            }
            Geometry(const Geometry<DimensionType>& another):pPoints(another.pPoints)
            {

            }
        //}

        // @ Destructor { 
            ~Geometry()
            {
                
            }    
        //}

        // @ Access { 
            PointType& GetValue(int index)
            {
                return pPoints.GetValue(index);
            }
            PointType GetValue(int index) const
            {
                return pPoints.GetValue(index);
            }
            Points_Container<PointType>& pPointsVector()
            {
                return pPoints;
            }
        //}

        // @ Utility { 
            static int GetType()
            {
                return 0;
            }
        //}
    private:
        Points_Container<PointType> pPoints;
        Geometry_Data<DimensionType> pGeometryData;
};

#endif