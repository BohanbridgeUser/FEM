#ifndef _QUADRAtURE_H_
#define _QUADRAtURE_H_

// @ This file defines template Quadrature with TDimensionType and TIntegrationPointsType
//   TDimensionType inplies the Dimension of model. TIntegrationPointsType inplies the IntegrationPointsType used
//   for calculating integraion.
//   
#include "../define.h"
#include "../Geometry/geometry_dimension.h"

template<typename TDimensionType, typename TIntegrationPointsType>
class Quadrature
{
    public:
        // @ Define { 
        typedef Quadrature<TDimensionType,TIntegrationPointsType> PointerDefine;
        LOTUS_POINTER_DEFINE(PointerDefine)
        typedef TDimensionType DimensionType;
        typedef TIntegrationPointsType IntegrationPointsType;
        //}
    private:

}

#endif