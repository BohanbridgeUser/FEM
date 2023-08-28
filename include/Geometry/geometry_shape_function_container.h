#ifndef _GEOMETRY_SHAPE_FUNCTION_CONTAINER_H_
#define _GEOMETRY_SHAPE_FUNCTION_CONTAINER_H_
#include "../define.h"
#include "../Quadrature/integration_point.h"
#include "geometry_data.h"

#include <vector>
#include <array>
#include <Eigen/Eigen>

template<typename TIntegrationMethodType>
class GeometryShapeFuncionContainer
{
    public:
        // @ Define {  
        LOTUS_POINTER_DEFINE(GeometryShapeFuncionContainer<TIntegrationMethodType>)
        typedef TIntegrationMethodType IntegrationMethod;

        // @ Integration Points Define
        typedef Integration_Point<3> 
                                                                        IntegrationPointType;
        typedef std::vector<IntegrationPointType> 
                                                                     IntegrationPointsVector;
        typedef std::array<IntegrationPointsVector, 
                            static_cast<int>(IntegrationMethod::NumberofIntegrationMethods)>
                                                              IntegrationPointsContainerType;
        
        // @ ShapeFunctionValueContainer Define
        typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                     ShapeFunctionsValueType;
        typedef std::array<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>,
                                static_cast<int>(IntegrationMethod::NumberofIntegrationMethods)> 
                                                            ShapeFunctionsValueContainerType;

        // @ First derivatives/gradients
        typedef std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >
                                                                 ShapeFunctionsGradientsType;
        typedef std::array<ShapeFunctionsGradientsType,
                                static_cast<int>(IntegrationMethod::NumberofIntegrationMethods)> 
                                                        ShapeFunctionsGradientsContainerType;
        //}

        // @ Constructor { 
        GeometryShapeFuncionContainer()
        {

        }
        GeometryShapeFuncionContainer(IntegrationMethod Method,
                                      const IntegrationPointsContainerType& ThisIntegrationPoints,
                                      const ShapeFunctionsValueContainerType& ThisShapeFunctionValueContainer,
                                      const ShapeFunctionsGradientsContainerType& ThisShapeFunctionsLocalGradients)
                                      :method(Method),
                                       mIntegrationPoints(ThisIntegrationPoints),
                                       mShapeFunctionValues(ThisShapeFunctionValueContainer),
                                       mShapeFunctionsLocalGradients(ThisShapeFunctionsLocalGradients)
        {

        }  
        //}

        // @ Destructor {
        ~GeometryShapeFuncionContainer()
        {

        }    
        //}

        // @ Access {
            IntegrationMethod& GetIntegrationMethod()
            {
                return method;
            }
            IntegrationPointsContainerType& GetIntegrationPoints()
            {
                return mIntegrationPoints;
            }
            ShapeFunctionsValueContainerType& GetShapeFunctionValues()
            {
                return mShapeFunctionValues;
            }
            const ShapeFunctionsValueType& ShapeFunctionsValues(IntegrationMethod ThisMethod)const
            {
                return mShapeFunctionValues.at(static_cast<int>(ThisMethod));
            }
            const ShapeFunctionsGradientsType& ShapeFunctionsLocalGradients( IntegrationMethod ThisMethod ) const
            {
                return mShapeFunctionsLocalGradients[static_cast<int>(ThisMethod)];
            }
            int GetIntegrationPointsNum(IntegrationMethod& ThisMethod)const
            {
                return mIntegrationPoints[static_cast<int>(ThisMethod)].size();
            }
            IntegrationMethod DefaultIntegrationMethod() const
            {
                return mDefaultMethod;
            }
            const IntegrationPointsVector& IntegrationPoints(const IntegrationMethod& ThisMethod)const
            {
                return mIntegrationPoints[static_cast<int>(ThisMethod)];
            }
        //}
    private:
        IntegrationMethod mDefaultMethod;
        IntegrationMethod method;
        IntegrationPointsContainerType mIntegrationPoints;
        ShapeFunctionsValueContainerType mShapeFunctionValues;
        ShapeFunctionsGradientsContainerType mShapeFunctionsLocalGradients;
};

#endif