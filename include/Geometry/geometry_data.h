#ifndef _GEOMETRY_DATA_H_
#define _GEOMETRY_DATA_H_
#include "geometry.h"
#include "geometry_dimension.h"
#include "geometry_shape_function_container.h"

class Geometry_Data
{
    public:
        // @ Define { 
        enum class IntegrationMethod{
            Gauss_Legendre_1,
            Gauss_Legendre_2,
            Gauss_Legendre_3,
            Gauss_Legendre_4,
            Gauss_Legendre_5,
            NumberofIntegrationMethods
        };

        // @ Integration Points Define
        typedef Integration_Point<3> 
                                                                                        IntegrationPointType;
        typedef std::vector<IntegrationPointType> 
                                                                                     IntegrationPointsVector;
        typedef std::array<IntegrationPointsVector, 
                            static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)>
                                                                              IntegrationPointsContainerType;
        // @ ShapeFunctionValueContainer Define
        typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                                     ShapeFunctionsValueType;
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
        Geometry_Data(const Geometry_Dimension* pThisDimension)
        :pGeometryDimension(pThisDimension)
        {

        }
        Geometry_Data(const Geometry_Dimension* pThisDimension,
                      const IntegrationMethod& rThisIntegrationMethod,
                      const IntegrationPointsContainerType& ThisPointsContainer, 
                      const ShapeFunctionsValueContainerType& ThisShapeFunctionValueContainer,
                      const ShapeFunctionsGradientsContainerType& ThisShapeFunctionsGradientsContainer)
        :pGeometryDimension(pThisDimension),
        mIntegrationMethod(rThisIntegrationMethod),
        mGeometryShapeFunctionContainer(rThisIntegrationMethod,
                                        ThisPointsContainer,
                                        ThisShapeFunctionValueContainer,
                                        ThisShapeFunctionsGradientsContainer)
        {
                
        }
        //}

        // @ Destructor {
        ~Geometry_Data()
        {
            
        }
        //}

        // @ Access {
            const GeometryShapeFuncionContainer<IntegrationMethod>& GetShapeInformation() const
            {
                return mGeometryShapeFunctionContainer;
            }
            const ShapeFunctionsGradientsType& ShapeFunctionsLocalGradients( IntegrationMethod ThisMethod ) const
            {
                return mGeometryShapeFunctionContainer.ShapeFunctionsLocalGradients(ThisMethod);
            }
            const ShapeFunctionsValueType& ShapeFunctionsValues(IntegrationMethod ThisMethod)const
            {
                return mGeometryShapeFunctionContainer.ShapeFunctionsValues(ThisMethod);
            }
            int GetIntegrationPointsNum(Geometry_Data::IntegrationMethod& ThisMethod)const
            {
                return mGeometryShapeFunctionContainer.GetIntegrationPointsNum(ThisMethod);
            }
            IntegrationMethod DefaultIntegrationMethod() const
            {
                return mGeometryShapeFunctionContainer.DefaultIntegrationMethod();
            }
            const IntegrationPointsVector& IntegrationPoints(const IntegrationMethod& ThisMethod)const
            {
                return mGeometryShapeFunctionContainer.IntegrationPoints(ThisMethod);
            }
            const IntegrationPointsVector& IntegrationPoints() const
            {
                return mGeometryShapeFunctionContainer.IntegrationPoints();
            }
        //}
    private:
        Geometry_Dimension const* pGeometryDimension;
        IntegrationMethod mIntegrationMethod;
        GeometryShapeFuncionContainer<IntegrationMethod> mGeometryShapeFunctionContainer;
};

#endif