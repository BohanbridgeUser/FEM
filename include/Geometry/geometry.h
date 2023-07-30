#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_
#include "../define.h"
#include "geometry_dimension.h"
#include "../Container/points_container.h"
#include "geometry_data.h"
#include "../Quadrature/integration_point.h"
#include <vector>
#include <array>
#include <memory>
template<class DimensionType>
class Geometry 
{
    public:
        // @ Define { 
        enum class GeometryType {
            Line,
            Triangle,
            Quadrilateral,
            Tetrahedron,
            Hexahedron
        };
        LOTUS_POINTER_DEFINE(Geometry)
        using PointType = Point<DimensionType::D>;

        // @ Integration Points Define
        typedef Integration_Point<3> IntegrationPointType;
        typedef std::vector<IntegrationPointType> IntegrationPointsVector;
        typedef std::array<IntegrationPointsVector, 
                            static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)>
                                IntegrationPointsContainerType;
        
        // @ ShapeFunctionValueContainer Define
        typedef std::array<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>,
                                static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)> 
                                    ShapeFunctionsValueContainerType;

        // @ First derivatives/gradients
        typedef std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >
                            ShapeFunctionsGradientsType;
        typedef std::array<ShapeFunctionsGradientsType,
                                static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)> 
                                    ShapeFunctionsGradientsContainerType;
        //}

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
            // static int GetType()
            // {
            //     return 0;
            // }
            int GetPointsNum()const
            {
                return pPoints.size();
            }
        //}
    private:
        Points_Container<PointType> pPoints;
        // static const Geometry_Data pGeometryData;
        // static const Geometry_Dimension pDimensiont;
};

#endif