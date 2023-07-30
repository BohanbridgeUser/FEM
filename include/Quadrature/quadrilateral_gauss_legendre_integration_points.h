#ifndef _QUADRILATERAL_GAUSS_LEGENDRE_INTEGRATION_POINTS_H_
#define _QUADRILATERAL_GAUSS_LEGENDRE_INTEGRATION_POINTS_H_
#include "../define.h"
#include "../Geometry/Point.h"
#include "integration_point.h"
#include <array>
class Quadrilateral_Gauss_Legendre_Integration_Points1
{
    public:
    
        LOTUS_POINTER_DEFINE(Quadrilateral_Gauss_Legendre_Integration_Points1)
        typedef Integration_Point<3> IntegrationPointsType;
        typedef std::array<IntegrationPointsType,1> IntegrationPointArray; 
        
    static int IntegrationPointsNumber()
    {
        return 1;
    }
    static const IntegrationPointArray& IntegrationPoints()
    {
        static const IntegrationPointArray array_integration_array{
            IntegrationPointsType(0.00,0.00,0.00,4.00)
        };
        return array_integration_array;
    }    
};

class Quadrilateral_Gauss_Legendre_Integration_Points2
{
    public:
        LOTUS_POINTER_DEFINE(Quadrilateral_Gauss_Legendre_Integration_Points2)
        typedef Integration_Point<3> IntegrationPointsType;
        typedef std::array<IntegrationPointsType, 4 > IntegrationPointArray; 

    public:
    static int IntegrationPointsNumber()
    {
        return 4;
    }
    static const IntegrationPointArray& IntegrationPoints()
    {
        static const IntegrationPointArray array_integration_array{
            IntegrationPointsType( -1.00/std::sqrt(3.0) , -1.00/std::sqrt(3.0),0.00, 1.00 ),
            IntegrationPointsType(  1.00/std::sqrt(3.0) , -1.00/std::sqrt(3.0),0.00, 1.00 ),
            IntegrationPointsType(  1.00/std::sqrt(3.0) ,  1.00/std::sqrt(3.0),0.00, 1.00 ),
            IntegrationPointsType( -1.00/std::sqrt(3.0) ,  1.00/std::sqrt(3.0),0.00, 1.00 )
        };
        return array_integration_array;
    }    
};
class Quadrilateral_Gauss_Legendre_Integration_Points3
{
    public:
        LOTUS_POINTER_DEFINE(Quadrilateral_Gauss_Legendre_Integration_Points3)
        typedef Integration_Point<3> IntegrationPointsType;
        typedef std::array<IntegrationPointsType, 9 > IntegrationPointArray; 
    public:
    static int IntegrationPointsNumber()
    {
        return 9;
    }
    static const IntegrationPointArray& IntegrationPoints()
    {
        static const IntegrationPointArray array_integration_array{
            IntegrationPointsType( -std::sqrt(3.00/5.00) , -std::sqrt(3.00/5.00), 0.00,  25.00/81.00 ),
            IntegrationPointsType(                  0.00 , -std::sqrt(3.00/5.00), 0.00,  40.00/81.00 ),
            IntegrationPointsType(  std::sqrt(3.00/5.00) , -std::sqrt(3.00/5.00), 0.00,  25.00/81.00 ),
            IntegrationPointsType( -std::sqrt(3.00/5.00) ,                  0.00, 0.00,  40.00/81.00 ),
            IntegrationPointsType(                  0.00 ,                  0.00, 0.00,  64.00/81.00 ),
            IntegrationPointsType(   std::sqrt(3.00/5.00),                  0.00, 0.00,  40.00/81.00 ),
            IntegrationPointsType(  -std::sqrt(3.00/5.00),  std::sqrt(3.00/5.00), 0.00,  25.00/81.00 ),
            IntegrationPointsType(                   0.00,  std::sqrt(3.00/5.00), 0.00,  40.00/81.00 ),
            IntegrationPointsType(   std::sqrt(3.00/5.00),  std::sqrt(3.00/5.00), 0.00,  25.00/81.00 )
        };
        return array_integration_array;
    }    
};
class Quadrilateral_Gauss_Legendre_Integration_Points4
{
    public:
        LOTUS_POINTER_DEFINE(Quadrilateral_Gauss_Legendre_Integration_Points4)
        typedef Integration_Point<3> IntegrationPointsType;
        typedef std::array<IntegrationPointsType, 16> IntegrationPointArray; 

    public:
    static int IntegrationPointsNumber()
    {
        return 16;
    }
    static const IntegrationPointArray& IntegrationPoints()
    {
        static const IntegrationPointArray array_integration_array{
            IntegrationPointsType( -std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), -std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 - std::sqrt(5.0/6.0)/6.0)*(0.5 - std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType( -std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), -std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 - std::sqrt(5.0/6.0)/6.0)*(0.5 + std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType( -std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ),  std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 - std::sqrt(5.0/6.0)/6.0)*(0.5 + std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType( -std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ),  std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 - std::sqrt(5.0/6.0)/6.0)*(0.5 - std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType( -std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), -std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 + std::sqrt(5.0/6.0)/6.0)*(0.5 - std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType( -std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), -std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 + std::sqrt(5.0/6.0)/6.0)*(0.5 + std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType( -std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ),  std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 + std::sqrt(5.0/6.0)/6.0)*(0.5 + std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType( -std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ),  std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 + std::sqrt(5.0/6.0)/6.0)*(0.5 - std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType(  std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), -std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 + std::sqrt(5.0/6.0)/6.0)*(0.5 - std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType(  std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), -std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 + std::sqrt(5.0/6.0)/6.0)*(0.5 + std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType(  std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ),  std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 + std::sqrt(5.0/6.0)/6.0)*(0.5 + std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType(  std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ),  std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 + std::sqrt(5.0/6.0)/6.0)*(0.5 - std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType(  std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), -std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 - std::sqrt(5.0/6.0)/6.0)*(0.5 - std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType(  std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), -std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 - std::sqrt(5.0/6.0)/6.0)*(0.5 + std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType(  std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ),  std::sqrt( (3.0 - 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 - std::sqrt(5.0/6.0)/6.0)*(0.5 + std::sqrt(5.0/6.0)/6.0)),
            IntegrationPointsType(  std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ),  std::sqrt( (3.0 + 2.0 * std::sqrt(6.0/5.0)) / 7.0 ), 0.00, (0.5 - std::sqrt(5.0/6.0)/6.0)*(0.5 - std::sqrt(5.0/6.0)/6.0))
        };
        return array_integration_array;
    }    
};
class Quadrilateral_Gauss_Legendre_Integration_Points5
{
    public:
        LOTUS_POINTER_DEFINE(Quadrilateral_Gauss_Legendre_Integration_Points5)
        typedef Integration_Point<3> IntegrationPointsType;
        typedef std::array<IntegrationPointsType, 25> IntegrationPointArray; 

    public:
    static int IntegrationPointsNumber()
    {
        return 25;
    }
    static const IntegrationPointArray& IntegrationPoints()
    {
        static IntegrationPointArray array_integration_array;
        const double a[] = {-0.906179845938664, -0.538469310105683, 0.000000000000000, 0.538469310105683, 0.906179845938664};
        const double w[] = {0.236926885056189, 0.478628670499366, 0.568888888888889, 0.478628670499366, 0.236926885056189};

        for(unsigned int i = 0; i < 5; ++i) {
            for(unsigned int j = 0; j < 5; ++j) {
                array_integration_array[5*i + j] = IntegrationPointsType( a[i], a[j], 0.00, w[i] * w[j]);
            }
        }
        return array_integration_array;
    }    
};
#endif