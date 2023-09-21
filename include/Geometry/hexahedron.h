#ifndef _HEXAHEDRON_H_
#define _HEXAHEDRON_H_
#include "geometry.h"
#include "../Container/geometry_container.h"
#include "Point.h"
#include "../Quadrature/hexahedron_gauss_legendre_integration_points.h"
#include "../Quadrature/quadrature.h"

/**
 *             v
 *      3----------2
 *      |\     ^   |\
 *      | \    |   | \
 *      |  \   |   |  \
 *      |   7------+---6
 *      |   |  +-- |-- | -> u
 *      0---+---\--1   |
 *       \  |    \  \  |
 *        \ |     \  \ |
 *         \|      w  \|
 *          4----------5
*/

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
            typedef PointerVector<PointType> 
                                                                  PointsContainerType;

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
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                          JacobianType;
            typedef std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > 
                                                                         JacobiansType;
              

        /// @}


        /// @name Life Circle
        /// @{
            Hexahedron()
            :GeometryType()
            {
            }
            Hexahedron(const int& NewId,PointsContainerType& points)
            :GeometryType(NewId,points,mGeometryData)
            {
            }
            Hexahedron(const int& NewId,TPointType& point1,
                       TPointType& point2,TPointType& point3,
                       TPointType& point4,TPointType& point5,
                       TPointType& point6,TPointType& point7,
                       TPointType& point8)
            :GeometryType(NewId,PointsContainerType(),mGeometryData)
            {
                this->pPointsVector().push_back(typename TPointType::Pointer(&point1));
                this->pPointsVector().push_back(typename TPointType::Pointer(&point2));
                this->pPointsVector().push_back(typename TPointType::Pointer(&point3));
                this->pPointsVector().push_back(typename TPointType::Pointer(&point4));
                this->pPointsVector().push_back(typename TPointType::Pointer(&point5));
                this->pPointsVector().push_back(typename TPointType::Pointer(&point6));
                this->pPointsVector().push_back(typename TPointType::Pointer(&point7));
                this->pPointsVector().push_back(typename TPointType::Pointer(&point8));
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
                int Integration_Points_Number = IntegrationPtsVector.size();
                ShapeFunctionValueType result(Integration_Points_Number,8);
                for (int i=0;i<Integration_Points_Number;++i)
                {
                    result(i,0) = 0.125 * (1-IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z());
                    result(i,1) = 0.125 * (1+IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z());
                    result(i,2) = 0.125 * (1+IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z());
                    result(i,3) = 0.125 * (1-IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z());
                    result(i,4) = 0.125 * (1-IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z());
                    result(i,5) = 0.125 * (1+IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z());
                    result(i,6) = 0.125 * (1+IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z());
                    result(i,7) = 0.125 * (1-IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z());
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
            static ShapeFunctionGradientsType CalculateShapeFunctionsGradient(Geometry_Data::IntegrationMethod const& rThisMethod)
            {
                IntegrationPointsContainerType all_integration_points = AllIntegrationPoints();
                IntegrationPointsVector IntegrationPtsVector = all_integration_points[static_cast<int>(rThisMethod)];
                const int integration_points_number = IntegrationPtsVector.size();
                ShapeFunctionGradientsType result( integration_points_number );
                for (int i=0;i<integration_points_number;++i)
                {
                    Eigen::Matrix<double,8,3> temp;
                    temp << -0.125 * (1-IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z()),
                            -0.125 * (1-IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].z()),
                            -0.125 * (1-IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()),

                            0.125 * (1-IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z()),
                           -0.125 * (1+IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].z()),
                           -0.125 * (1+IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()),

                            0.125 * (1+IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z()),
                            0.125 * (1+IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].z()),
                           -0.125 * (1+IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y()),
                            
                           -0.125 * (1+IntegrationPtsVector[i].y()) * (1-IntegrationPtsVector[i].z()),
                            0.125 * (1-IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].z()),
                           -0.125 * (1-IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y()),

                           -0.125 * (1-IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z()),
                           -0.125 * (1-IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].z()),
                            0.125 * (1-IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()),

                            0.125 * (1-IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z()),
                           -0.125 * (1+IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].z()),
                            0.125 * (1+IntegrationPtsVector[i].x()) * (1-IntegrationPtsVector[i].y()),

                            0.125 * (1+IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z()),
                            0.125 * (1+IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].z()),
                            0.125 * (1+IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y()),

                           -0.125 * (1+IntegrationPtsVector[i].y()) * (1+IntegrationPtsVector[i].z()),
                            0.125 * (1-IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].z()),
                            0.125 * (1-IntegrationPtsVector[i].x()) * (1+IntegrationPtsVector[i].y());
                    result[i] = temp;
                };
                return result;
            }
            static const ShapeFunctionsGradientsContainerType AllShapeFunctionsGradients()
            {
                ShapeFunctionsGradientsContainerType result
                {
                    Hexahedron<TPointType>::CalculateShapeFunctionsGradient(Geometry_Data::IntegrationMethod::Gauss_Legendre_1),
                    Hexahedron<TPointType>::CalculateShapeFunctionsGradient(Geometry_Data::IntegrationMethod::Gauss_Legendre_2),
                    Hexahedron<TPointType>::CalculateShapeFunctionsGradient(Geometry_Data::IntegrationMethod::Gauss_Legendre_3),
                    Hexahedron<TPointType>::CalculateShapeFunctionsGradient(Geometry_Data::IntegrationMethod::Gauss_Legendre_4),
                    Hexahedron<TPointType>::CalculateShapeFunctionsGradient(Geometry_Data::IntegrationMethod::Gauss_Legendre_5)
                };
                return result;
            }
            /**
             * Calculates the gradients in terms of local coordinateds
             * of all shape functions in a given point.
             *
             * @param rPoint the current point at which the gradients are calculated
             * @return the gradients of all shape functions
             * \f$ \frac{\partial N^i}{\partial \xi_j} \f$
             */
            Matrix& ShapeFunctionsLocalGradients( Matrix& rResult, PointType const& rPoint ) const override
            {
                if ( rResult.rows() != 8 || rResult.cols() != 3 )
                    rResult.resize( 8, 3);
                rResult( 0, 0 ) = -0.125 * ( 1.0 - rPoint[1] ) * ( 1.0 - rPoint[2] );
                rResult( 0, 1 ) = -0.125 * ( 1.0 - rPoint[0] ) * ( 1.0 - rPoint[2] );
                rResult( 0, 2 ) = -0.125 * ( 1.0 - rPoint[0] ) * ( 1.0 - rPoint[1] );
                rResult( 1, 0 ) =  0.125 * ( 1.0 - rPoint[1] ) * ( 1.0 - rPoint[2] );
                rResult( 1, 1 ) = -0.125 * ( 1.0 + rPoint[0] ) * ( 1.0 - rPoint[2] );
                rResult( 1, 2 ) = -0.125 * ( 1.0 + rPoint[0] ) * ( 1.0 - rPoint[1] );
                rResult( 2, 0 ) =  0.125 * ( 1.0 + rPoint[1] ) * ( 1.0 - rPoint[2] );
                rResult( 2, 1 ) =  0.125 * ( 1.0 + rPoint[0] ) * ( 1.0 - rPoint[2] );
                rResult( 2, 2 ) = -0.125 * ( 1.0 + rPoint[0] ) * ( 1.0 + rPoint[1] );
                rResult( 3, 0 ) = -0.125 * ( 1.0 + rPoint[1] ) * ( 1.0 - rPoint[2] );
                rResult( 3, 1 ) =  0.125 * ( 1.0 - rPoint[0] ) * ( 1.0 - rPoint[2] );
                rResult( 3, 2 ) = -0.125 * ( 1.0 - rPoint[0] ) * ( 1.0 + rPoint[1] );
                rResult( 4, 0 ) = -0.125 * ( 1.0 - rPoint[1] ) * ( 1.0 + rPoint[2] );
                rResult( 4, 1 ) = -0.125 * ( 1.0 - rPoint[0] ) * ( 1.0 + rPoint[2] );
                rResult( 4, 2 ) =  0.125 * ( 1.0 - rPoint[0] ) * ( 1.0 - rPoint[1] );
                rResult( 5, 0 ) =  0.125 * ( 1.0 - rPoint[1] ) * ( 1.0 + rPoint[2] );
                rResult( 5, 1 ) = -0.125 * ( 1.0 + rPoint[0] ) * ( 1.0 + rPoint[2] );
                rResult( 5, 2 ) =  0.125 * ( 1.0 + rPoint[0] ) * ( 1.0 - rPoint[1] );
                rResult( 6, 0 ) =  0.125 * ( 1.0 + rPoint[1] ) * ( 1.0 + rPoint[2] );
                rResult( 6, 1 ) =  0.125 * ( 1.0 + rPoint[0] ) * ( 1.0 + rPoint[2] );
                rResult( 6, 2 ) =  0.125 * ( 1.0 + rPoint[0] ) * ( 1.0 + rPoint[1] );
                rResult( 7, 0 ) = -0.125 * ( 1.0 + rPoint[1] ) * ( 1.0 + rPoint[2] );
                rResult( 7, 1 ) =  0.125 * ( 1.0 - rPoint[0] ) * ( 1.0 + rPoint[2] );
                rResult( 7, 2 ) =  0.125 * ( 1.0 - rPoint[0] ) * ( 1.0 + rPoint[1] );
                return rResult;
            }

        /// @}


        /// @name Access
        /// @{
            static int GetType()
            {
                return (int)Geometry<TPointType>::Geometry_Type::Hexahedron;
            }

        /// @}


        /// @name Inquiry
        /// @{


        /// @}

        /// @name Input And Output
        /// @{
            /**
             * Turn back information as a string.
             *
             * @return String contains information about this geometry.
             * @see PrintData()
             * @see PrintInfo()
             */
            std::string Info() const override
            {
                return "3 dimensional hexahedra with eight nodes in 3D space";
            }
            /**
             * Print information about this object.
             *
             * @param rOStream Stream to print into it.
             * @see PrintData()
             * @see Info()
             */
            void PrintInfo( std::ostream& rOStream ) const override
            {
                rOStream << "3 dimensional hexahedra with eight nodes in 3D space";
            }
            /**
             * Print geometry's data into given stream.
             * Prints it's points by the order they stored in the geometry
             * and then center point of geometry.
             *
             * @param rOStream Stream to print into it.
             * @see PrintInfo()
             * @see Info()
             */
            void PrintData( std::ostream& rOStream ) const override
            {
                // Base Geometry class PrintData call
                GeometryType::PrintData( rOStream );
                std::cout << std::endl;

                // If the geometry has valid points, calculate and output its data
                // if (this->AllPointsAreValid()) {
                    JacobiansType jacobian;
                    this->Jacobian( jacobian);
                    for (auto it=jacobian.begin();it!=jacobian.end();++it)
                    {
                        rOStream << "\n    Jacobian at integration point     " << it-jacobian.begin() << " \n" << *it;
                    }
                    
                    std::cout << std::endl;
                // }
            }
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

/**
 * output stream function
 */
template<class TPointType> 
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Hexahedron<TPointType>& rThis )
{
    rThis.PrintInfo( rOStream );
    rOStream << std::endl;
    rThis.PrintData( rOStream );

    return rOStream;
}
#endif
