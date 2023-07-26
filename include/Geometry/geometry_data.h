#ifndef _GEOMETRY_DATA_H_
#define _GEOMETRY_DATA_H_

template<class DimensionType>
class Geometry_Data
{
    public:
        enum class IntegrationMethod{
            Gauss_Legendre_1,
            Gauss_Legendre_2,
            Gauss_Legendre_3,
            Gauss_Legendre_4,
            Gauss_Legendre_5,
            Gauss_Lobatto_1,
            Gauss_Lobatto_2,
            Gauss_Lobatto_3,
            Gauss_Lobatto_4,
            Gauss_Lobatto_5
        };

        // @ Constructor { 
        Geometry_Data()
        {

        }
        //}

        // @ Destructor {
        ~Geometry_Data()
        {
            
        }
        //}
    private:
        IntegrationMethod mIntegrationMethod;
        // GeometryShapeFunctionContainer<IntegrationMethod> mGeometryShapeFunctionContainer;
};

#endif