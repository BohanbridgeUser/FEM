#ifndef _HEXAHEDRON_H_
#define _HEXAHEDRON_H_
#include "geometry.h"
#include "../Container/geometry_container.h"
#include "Point.h"
#include "../Quadrature/hexahedron_gauss_legendre_integration_points.h"

template <typename TPointType>
class Hexahedron:public Geometry<TPointType>
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Hexahedron<TPointType>)
            typedef Geometry<TPointType>
                                                                         GeometryType;
            typedef TPointType 
                                                                            PointType;
            typedef std::vector<PointType> 
                                                                     PointsVectorType;
            typedef std::vector<PointType> 
                                                                      PointsContainer;

            // @ Integration Points Define
            typedef Geometry_Data::IntegrationMethod
                                                                    IntegrationMethod;
            typedef Integration_Point<3> 
                                                                 IntegrationPointType;
            typedef std::vector<IntegrationPointType> 
                                                              IntegrationPointsVector;
            typedef std::array<IntegrationPointsVector, 
                                static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)>
                                                       IntegrationPointsContainerType;
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                               Matrix;
            typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                                               Vector;
            // @ ShapeFunctionValueContainer Define
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                               ShapeFunctionValueType;
            typedef std::array<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>,
                                    static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)> 
                                                     ShapeFunctionsValueContainerType;

            // @ First derivatives/gradients
            typedef std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >
                                                          ShapeFunctionGradientsType;
            typedef std::array<ShapeFunctionGradientsType,
                                    static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)> 
                                                  ShapeFunctionsGradientsContainerType;

            /* JacobiansType */
            typedef std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > 
                                                                         JacobiansType;
              

        /// @}


        /// @name Life Circle
        /// @{
            Hexahedron()
            :GeometryType()
            {
            }
            Hexahedron(const int& NewId,PointsContainer& points)
            :GeometryType(NewId,points,mGeometryData)
            {
            }
            Hexahedron(const Hexahedron<TPointType>& another)
            :GeometryType(another)
            {
            }
            Hexahedron(Hexahedron<TPointType>&& another)
            :GeometryType(another)
            {
            }

            ~Hexahedron()
            {
            }    
        //}

        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            static const IntegrationPointsContainerType AllIntegrationPoints()
            {
                IntegrationPointsContainerType result
                {
                    Quadrature<Hexahedron_Gauss_Legendre_Integration_Points1,TPointType,IntegrationPointType>::GenerateIntegrationPoints(),
                    Quadrature<Hexahedron_Gauss_Legendre_Integration_Points2,TPointType,IntegrationPointType>::GenerateIntegrationPoints(),
                    Quadrature<Hexahedron_Gauss_Legendre_Integration_Points3,TPointType,IntegrationPointType>::GenerateIntegrationPoints(),
                    Quadrature<Hexahedron_Gauss_Legendre_Integration_Points4,TPointType,IntegrationPointType>::GenerateIntegrationPoints(),
                    Quadrature<Hexahedron_Gauss_Legendre_Integration_Points5,TPointType,IntegrationPointType>::GenerateIntegrationPoints(),
                };
                return result;
            }
            static ShapeFunctionValueType CalculateShapeFunctionValue(Geometry_Data::IntegrationMethod const& rThisMethod)
            {
                const IntegrationPointsContainerType IntegrationPts = AllIntegrationPoints();
                const IntegrationPointsVector IntegrationPtsVector = IntegrationPts[static_cast<int>(rThisMethod)];
                int Integration_Points_Number = mGeometryData.GetIntegrationPointsNum();
                ShapeFunctionValueType result(Integration_Points_Number,8);
                for (int i=0;i<Integration_Points_Number;++i)
                {
                    result[i,0] = 0.125 * (1-IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z());
                    result[i,1] = 0.125 * (1+IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z());
                    result[i,2] = 0.125 * (1+IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z());
                    result[i,3] = 0.125 * (1-IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z());
                    result[i,4] = 0.125 * (1-IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z());
                    result[i,5] = 0.125 * (1+IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z());
                    result[i,6] = 0.125 * (1+IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z());
                    result[i,7] = 0.125 * (1-IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z());
                } 
                return result;
            }
            static const ShapeFunctionsValueContainerType AllShapeFunctionsValues()
            {
                ShapeFunctionsValueContainerType result
                {
                    Hexahedron<TPointType>::CalculateShapeFunctionValue(Geometry_Data::IntegrationMethod::Gauss_Legendre_1),
                    Hexahedron<TPointType>::CalculateShapeFunctionValue(Geometry_Data::IntegrationMethod::Gauss_Legendre_2),
                    Hexahedron<TPointType>::CalculateShapeFunctionValue(Geometry_Data::IntegrationMethod::Gauss_Legendre_3),
                    Hexahedron<TPointType>::CalculateShapeFunctionValue(Geometry_Data::IntegrationMethod::Gauss_Legendre_4),
                    Hexahedron<TPointType>::CalculateShapeFunctionValue(Geometry_Data::IntegrationMethod::Gauss_Legendre_5)
                };
                return result;
            }
        /// @}


        /// @name Access
        /// @{
            int GetID() const 
            {
                return ID;
            }
            static int GetType()
            {
                return (int)Geometry<TPointType>::Geometry_Type::Hexahedron;
            }

        /// @}


        /// @name Inquiry
        /// @{


        /// @}


    protected:
        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{


        /// @}


        /// @name Protected Operatiors
        /// @{


        /// @}


        /// @name Protected Operations
        /// @{


        /// @}


        /// @name Protected Access
        /// @{


        /// @}


        /// @name Protected Inquiry
        /// @{


        /// @}


    private:
        /// @name Private Static Member Variables
        /// @{
            static const Geometry_Data mGeometryData;
            static const Geometry_Dimension mGeometryDimension;

        /// @}


        /// @name Private Member Variables
        /// @{


        /// @}


        /// @name Private Operatiors
        /// @{


        /// @}


        /// @name Private Operations
        /// @{


        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};
template<typename TPointType>
Geometry_Dimension const Hexahedron<TPointType>::mGeometryDimension(3,3);

template<typename TPointType>
Geometry_Data const Hexahedron<TPointType>::mGeometryData
(
    &mGeometryDimension,
    Geometry_Data::IntegrationMethod::Gauss_Legendre_2,
    Hexahedron<TPointType>::AllIntegrationPoints(),
    Hexahedron<TPointType>::AllShapeFunctionsValues(),
    Hexahedron<TPointType>::AllShapeFunctionsGradients()
);
#endif
