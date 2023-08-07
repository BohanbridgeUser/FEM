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
template<class TPointType>
class Geometry 
{
    public:
        ///@ Define { 
        enum class Geometry_Type {
            Origin,
            Line,
            Triangle,
            Quadrilateral,
            Tetrahedron,
            Hexahedron
        };
        LOTUS_POINTER_DEFINE(Geometry)
        typedef TPointType PointType;

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

        ///@ Life Circle 
        ///@{ 
        // Constructor  
            Geometry()
            {
               std::vector<PointType> pPoints;
               number++;
               ID = number;
            }    
            Geometry(const int& id,Points_Container<PointType>& Points):pPoints(Points),ID(id)
            {
               number++;
            }
            Geometry(Points_Container<PointType>& Points):pPoints(Points)
            {
               number++;
               ID = number;
            }
            Geometry(const Geometry<TPointType>& another):pPoints(another.pPoints)
            {
               number++;
               ID = number;
            }
        // Destructor {
            ~Geometry()
            {
                if (number>0) number--;
            }    
        ///@}

        ///@name Access 
        ///@{
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
            int& GetID()
            {
                return ID;
            }
            int GetID() const
            {
                return ID;
            }
            static Geometry_Type GetType()
            {
                return GeometryType;
            }
        ///@}

        ///@ Utility { 
            // static int GetType()
            // {
            //     return 0;
            // }
            int GetPointsNum()const
            {
                return pPoints.GetPointsNum();
            }
        //}
    protected:
        int ID;
    private:
        Points_Container<PointType> pPoints;

        static Geometry_Type GeometryType; 
        static int number;
        static const Geometry_Data mGeometry_Data;
};
template<typename TPointType> int Geometry<TPointType>::number = 0;
template<typename TPointType> typename Geometry<TPointType>::Geometry_Type Geometry<TPointType>::GeometryType = Geometry_Type::Origin;
#endif