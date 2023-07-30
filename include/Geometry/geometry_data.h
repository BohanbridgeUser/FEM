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
        Geometry_Data()
        {

        }
        Geometry_Data(IntegrationMethod Method,
                        const IntegrationPointsContainerType& ThisPointsContainer, 
                        const ShapeFunctionsValueContainerType& ThisShapeFunctionValueContainer,
                        const ShapeFunctionsGradientsContainerType& ThisShapeFunctionsGradientsContainer)
        :mIntegrationMethod(Method),
        mGeometryShapeFunctionContainer(Method,
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
        GeometryShapeFuncionContainer<IntegrationMethod>& GetShapeInformation() 
        {
            return mGeometryShapeFunctionContainer;
        }
        int GetIntegrationPointsNum(Geometry_Data::IntegrationMethod& ThisMethod)const
        {
            return mGeometryShapeFunctionContainer.GetIntegrationPointsNum(ThisMethod);
        }
        //}
    private:
        IntegrationMethod mIntegrationMethod;
        GeometryShapeFuncionContainer<IntegrationMethod> mGeometryShapeFunctionContainer;
};

#endif