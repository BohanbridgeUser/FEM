#ifndef _QUADRAtURE_H_
#define _QUADRAtURE_H_

// @ This file defines template Quadrature with TDimensionType and TIntegrationPointsType
//   TDimensionType inplies the Dimension of model. TIntegrationPointsType inplies the IntegrationPointsType used
//   for calculating integraion.
//   
#include "../define.h"
#include "../Geometry/geometry_dimension.h"
#include "integration_point.h"
#include <vector>
#include <cmath>
template<typename TQuadratureType, typename TDimensionType, typename TIntegrationPointsType>
class Quadrature
{
    public:
        // @ Define { 
        typedef Quadrature<TQuadratureType,TDimensionType,TIntegrationPointsType> PointerDefine;
        LOTUS_POINTER_DEFINE(PointerDefine)
        typedef TDimensionType DimensionType;
        typedef TIntegrationPointsType IntegrationPointsType;
        typedef std::vector<IntegrationPointsType> IntegrationPointsVector;
        //}

        // @ Constructor { 
        Quadrature()
        {

        }
        // }

        // @ Destructor {
        ~Quadrature()
        {

        }
        //}

        static IntegrationPointsVector GenerateIntegrationPoints()
        {
            IntegrationPointsVector result;
            IntegrationPoints(result,
                                    Quadrature<TQuadratureType, TDimensionType, TIntegrationPointsType>());
            return result;               
        }
        static void IntegrationPoints(IntegrationPointsVector& result,
                                      const Quadrature<TQuadratureType, TDimensionType, TIntegrationPointsType>& dummy )
        {
            int Dimensions = TDimensionType::D;
            int IntegrationPointsNum = TQuadratureType::IntegrationPointsNumber();
            auto TIntegrationPointsVector = TQuadratureType::IntegrationPoints();
            for (int i=0;i<IntegrationPointsNum;++i) {
                result.push_back(TIntegrationPointsVector[i]);
            }
        }
    private:

};

#endif