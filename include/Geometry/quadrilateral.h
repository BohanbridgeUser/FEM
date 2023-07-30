#ifndef _QUADRILATERAL_H_
#define _QUADRILATERAL_H_
#include "geometry.h"
#include "geometry_dimension.h"
#include "../Container/geometry_container.h"
#include "../Quadrature/integration_point.h"
#include "../Quadrature/quadrilateral_gauss_legendre_integration_points.h"
#include "../Quadrature/quadrature.h"
#include "Point.h"
template <typename DimensionType>
class Quadrilateral:public Geometry<DimensionType>
{
    public:
        // @ Define { 
        LOTUS_POINTER_DEFINE(Quadrilateral<DimensionType>)
        typedef Point<DimensionType::D> PointType;
        typedef std::vector<PointType> PointsVectorType;
        typedef Points_Container<PointType> PointsContainer;

        // @ Integration Points Define
        typedef Integration_Point<3> IntegrationPointType;
        typedef std::vector<IntegrationPointType> IntegrationPointsVector;
        typedef std::array<IntegrationPointsVector, 
                            static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)>
                                IntegrationPointsContainerType;
        
        // @ ShapeFunctionValueContainer Define
        typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> ShapeFunctionsValueType;
        typedef std::array<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>,
                                static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)> 
                                    ShapeFunctionsValueContainerType;

        // @ First derivatives/gradients
        typedef std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >
                            ShapeFunctionsGradientsType;
        typedef std::array<ShapeFunctionsGradientsType,
                                static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)> 
                                    ShapeFunctionsGradientsContainerType;

        // @ Jacobian 
        typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> JacobianType;
        typedef std::vector<JacobianType> JacobiansContainer;
        //}

        // @ Constructor { 
        Quadrilateral()
        {
            number++;
        }
        Quadrilateral(PointsContainer& points):Geometry<DimensionType>(points)
        {
            number++;
            ID = number;
        }
        Quadrilateral(const int& F,PointsContainer& points):Geometry<DimensionType>(points),ID(F)
        {
            number++;
        }
        Quadrilateral(const Quadrilateral<DimensionType>& another):Geometry<DimensionType>(another)
        {
            ID = ++number;
        }
        //}

        // @ Destructor { 
        ~Quadrilateral()
        {
            if (number > 0) number--;
        }    
        //}

        // @ Calculate Values { 
        static const IntegrationPointsContainerType AllIntegrationPoints()
        {
            IntegrationPointsContainerType result = 
            {
                {
                Quadrature<Quadrilateral_Gauss_Legendre_Integration_Points1,DimensionType,IntegrationPointType>::GenerateIntegrationPoints(),
                Quadrature<Quadrilateral_Gauss_Legendre_Integration_Points2,DimensionType,IntegrationPointType>::GenerateIntegrationPoints(),
                Quadrature<Quadrilateral_Gauss_Legendre_Integration_Points3,DimensionType,IntegrationPointType>::GenerateIntegrationPoints(),
                Quadrature<Quadrilateral_Gauss_Legendre_Integration_Points4,DimensionType,IntegrationPointType>::GenerateIntegrationPoints(),
                Quadrature<Quadrilateral_Gauss_Legendre_Integration_Points5,DimensionType,IntegrationPointType>::GenerateIntegrationPoints()
                }
            };
            return result;
        }
        static ShapeFunctionsValueType CalculateShapeFunctionsValue(Geometry_Data::IntegrationMethod ThisMethod)
        {
            IntegrationPointsContainerType IntegrationPs = AllIntegrationPoints();
            IntegrationPointsVector Vector_Integration_Points = IntegrationPs[static_cast<int>(ThisMethod)];
            int point_num = 4;
            int IntegrationPoints_num = Vector_Integration_Points.size();
            ShapeFunctionsValueType result;
            result.resize(IntegrationPoints_num, 4);
            for (int i=0;i<IntegrationPoints_num;++i) {
                result(i,0) = 0.25 * (1.0 - Vector_Integration_Points[i].x()) * (1.0 - Vector_Integration_Points[i].y());
                result(i,1) = 0.25 * (1.0 + Vector_Integration_Points[i].x()) * (1.0 - Vector_Integration_Points[i].y());
                result(i,2) = 0.25 * (1.0 + Vector_Integration_Points[i].x()) * (1.0 + Vector_Integration_Points[i].y());
                result(i,3) = 0.25 * (1.0 - Vector_Integration_Points[i].x()) * (1.0 + Vector_Integration_Points[i].y());
            }
            return result;
        }
        static const ShapeFunctionsValueContainerType AllShapeFunctionsValues()
        {
            ShapeFunctionsValueContainerType result
            {
                Quadrilateral<DimensionType>::CalculateShapeFunctionsValue(Geometry_Data::IntegrationMethod::Gauss_Legendre_1),
                Quadrilateral<DimensionType>::CalculateShapeFunctionsValue(Geometry_Data::IntegrationMethod::Gauss_Legendre_2),
                Quadrilateral<DimensionType>::CalculateShapeFunctionsValue(Geometry_Data::IntegrationMethod::Gauss_Legendre_3),
                Quadrilateral<DimensionType>::CalculateShapeFunctionsValue(Geometry_Data::IntegrationMethod::Gauss_Legendre_4),
                Quadrilateral<DimensionType>::CalculateShapeFunctionsValue(Geometry_Data::IntegrationMethod::Gauss_Legendre_5)
            };
            return result;
        }
        static ShapeFunctionsGradientsType CalculateShapeFunctionGradientsType(Geometry_Data::IntegrationMethod ThisMethod)
        {
            IntegrationPointsContainerType IntegrationPs = AllIntegrationPoints();
            IntegrationPointsVector Vector_Integration_Points = IntegrationPs[static_cast<int>(ThisMethod)];
            int point_num = 4;
            int IntegrationPoints_num = Vector_Integration_Points.size();
            ShapeFunctionsGradientsType result(IntegrationPoints_num);
            for (int i=0;i<IntegrationPoints_num;++i) {
                Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> temp {
                    {
                        -0.25 * (1 - Vector_Integration_Points[i].y()),
                        -0.25 * (1 - Vector_Integration_Points[i].x())
                    },
                    {
                        0.25 * (1 - Vector_Integration_Points[i].y()),
                        -0.25 * (1 + Vector_Integration_Points[i].x())
                    },
                    {
                        0.25 * (1 + Vector_Integration_Points[i].y()),
                        0.25 * (1 + Vector_Integration_Points[i].x()),
                    },
                    {
                        -0.25 * (1 + Vector_Integration_Points[i].y()),
                        0.25 * (1 - Vector_Integration_Points[i].x())
                    }
                };     
                result[i] = temp;
            }
            return result;
        }
        static ShapeFunctionsGradientsContainerType AllShapeFunctionsLocalGradients()
        {
            ShapeFunctionsGradientsContainerType result
            {
                Quadrilateral<DimensionType>::CalculateShapeFunctionGradientsType(Geometry_Data::IntegrationMethod::Gauss_Legendre_1),
                Quadrilateral<DimensionType>::CalculateShapeFunctionGradientsType(Geometry_Data::IntegrationMethod::Gauss_Legendre_2),
                Quadrilateral<DimensionType>::CalculateShapeFunctionGradientsType(Geometry_Data::IntegrationMethod::Gauss_Legendre_3),
                Quadrilateral<DimensionType>::CalculateShapeFunctionGradientsType(Geometry_Data::IntegrationMethod::Gauss_Legendre_4),
                Quadrilateral<DimensionType>::CalculateShapeFunctionGradientsType(Geometry_Data::IntegrationMethod::Gauss_Legendre_5)
            };
            return result;
        }
        JacobiansContainer& Jacobians(JacobiansContainer& result, Geometry_Data::IntegrationMethod ThisMethod)
        {
            ShapeFunctionsGradientsType S_F_G_T = CalculateShapeFunctionGradientsType(ThisMethod);
            int IntegrationsPointsNum = GetIntegrationPointsNums(ThisMethod);
            for (int i=0;i<IntegrationsPointsNum;++i) {
                Eigen::Matrix<double, 2, 2> J;
                for (int j=0;j<GetPointsNum();++j) {
                    J(0,0) += (this->GetPoint(j).x()) * S_F_G_T[i](j,0);
                    J(0,1) += (this->GetPoint(j).x()) * S_F_G_T[i](j,1);
                    J(1,0) += (this->GetPoint(j).y()) * S_F_G_T[i](j,0);
                    J(1,1) += (this->GetPoint(j).y()) * S_F_G_T[i](j,1);
                }
                result[i] = J;
            }
            return result;
        }
        //}

        // @ Utility {
        int GetID() const 
        {
            return ID;
        }
        static int GetType()
        {
            return (int)Geometry<DimensionType>::GeometryType::Quadrilateral;
        }
        //}

        // @ Access { 
        static const Geometry_Data& GetGeometryData()
        {
            return mGeometry_Data;
        }
        int GetIntegrationPointsNums(Geometry_Data::IntegrationMethod& ThisMethod)
        {
            return mGeometry_Data.GetIntegrationPointsNum(ThisMethod);
        }
        int GetPointsNum()
        {
            return 4;
        }
        PointType& GetPoint(int i)
        {
            return this->pPointsVector().GetValue(i);
        }
        //}
    private:
        int ID;
        static int number;
        static const Geometry_Data mGeometry_Data;
};
template <typename DimensionType> int Quadrilateral<DimensionType>::number = 0;

template <typename DimensionType> const 
Geometry_Data Quadrilateral<DimensionType>::mGeometry_Data(
Geometry_Data::IntegrationMethod::Gauss_Legendre_2,
Quadrilateral<DimensionType>::AllIntegrationPoints(),
Quadrilateral<DimensionType>::AllShapeFunctionsValues(),
Quadrilateral<DimensionType>::AllShapeFunctionsLocalGradients()
);

#endif