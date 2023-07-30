#ifndef _LINE_GAUSS_LEGENDRE_INTEGRATION_POINTS_H_
#define _LINE_GAUSS_LEGENDRE_INTEGRATION_POINTS_H_
#include "../define.h"
#include "../Geometry/Point.h"
#include "integration_point.h"
#include <array>
class Line_Gauss_Legendre_Integration_Points1
{
    public:
    
        LOTUS_POINTER_DEFINE(Line_Gauss_Legendre_Integration_Points1)
        typedef Integration_Point<1> IntegrationPointsType;
        typedef std::array<IntegrationPointsType,1> IntegrationPointArray; 
        
    static int IntegrationPointsNumber()
    {
        return 1;
    }
    static const IntegrationPointArray& IntegrationPoints()
    {
        static const IntegrationPointArray array_integration_array{
            IntegrationPointsType(0.00,0.00,0.00,2.00)
        };
        return array_integration_array;
    }    
};

class Line_Gauss_Legendre_Integration_Points2
{
    public:
        LOTUS_POINTER_DEFINE(Line_Gauss_Legendre_Integration_Points2)
        typedef Integration_Point<1> IntegrationPointsType;
        typedef std::array<IntegrationPointsType, 2 > IntegrationPointArray; 

    public:
    static int IntegrationPointsNumber()
    {
        return 2;
    }
    static const IntegrationPointArray& IntegrationPoints()
    {
        static const IntegrationPointArray array_integration_array{
            IntegrationPointsType(-0.5773502691896260,0.00,0.00,1.00),
            IntegrationPointsType(0.5773502691896260,0.00,0.00,1.00)
        };
        return array_integration_array;
    }    
};
class Line_Gauss_Legendre_Integration_Points3
{
    public:
        LOTUS_POINTER_DEFINE(Line_Gauss_Legendre_Integration_Points2)
        typedef Integration_Point<1> IntegrationPointsType;
        typedef std::array<IntegrationPointsType, 3 > IntegrationPointArray; 
    public:
    static int IntegrationPointsNumber()
    {
        return 3;
    }
    static const IntegrationPointArray& IntegrationPoints()
    {
        static const IntegrationPointArray array_integration_array{
            IntegrationPointsType(-0.7745966692414830,0.00,0.00,0.5555555555555560),
            IntegrationPointsType(0.0000000000000000,0.00,0.00,0.8888888888888890),
            IntegrationPointsType(0.7745966692414830,0.00,0.00,0.5555555555555560),
        };
        return array_integration_array;
    }    
};
class Line_Gauss_Legendre_Integration_Points4
{
    public:
        LOTUS_POINTER_DEFINE(Line_Gauss_Legendre_Integration_Points2)
        typedef Integration_Point<1> IntegrationPointsType;
        typedef std::array<IntegrationPointsType, 4> IntegrationPointArray; 

    public:
    static int IntegrationPointsNumber()
    {
        return 4;
    }
    static const IntegrationPointArray& IntegrationPoints()
    {
        static const IntegrationPointArray array_integration_array{
            IntegrationPointsType(-0.8611363115940530,0.00,0.00,0.3478548451374540),
            IntegrationPointsType(-0.3399810435848560,0.00,0.00,0.6521451548625460),
            IntegrationPointsType(0.3399810435848560,0.00,0.00,0.6521451548625460),
            IntegrationPointsType(0.7745966692414830,0.00,0.00,0.3478548451374540),
        };
        return array_integration_array;
    }    
};
class Line_Gauss_Legendre_Integration_Points5
{
    public:
        LOTUS_POINTER_DEFINE(Line_Gauss_Legendre_Integration_Points2)
        typedef Integration_Point<1> IntegrationPointsType;
        typedef std::array<IntegrationPointsType, 5> IntegrationPointArray; 

    public:
    static int IntegrationPointsNumber()
    {
        return 5;
    }
    static const IntegrationPointArray& IntegrationPoints()
    {
        static const IntegrationPointArray array_integration_array{
            IntegrationPointsType(-0.9061798459386640,0.00,0.00,0.2369268850561890),
            IntegrationPointsType(-0.5384693101056830,0.00,0.00,0.4786286704993660),
            IntegrationPointsType(0.0000000000000000,0.00,0.00,0.5688888888888890),
            IntegrationPointsType(0.5384693101056830,0.00,0.00,0.4786286704993660),
            IntegrationPointsType(0.9061798459386640,0.00,0.00,0.2369268850561890),
        };
        return array_integration_array;
    }    
};
#endif