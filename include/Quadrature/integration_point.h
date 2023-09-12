#ifndef _INTEGRATION_POINT_H_
#define _INTEGRATION_POINT_H_
#include "../Geometry/Point.h"

template<int TDimension>
class Integration_Point:public Point<3>
{
    public:
        // @ Constructor {
        /* Default */
        typedef Point<3>
                                                            PointType;
        Integration_Point()
        {
            
        }
        Integration_Point(const Integration_Point& another)
        :Point<3>(another),mWeight(another.mWeight)
        {

        }
        Integration_Point(const double& xi, const double& yi=0.0, const double& zi=0.0, const double& w = 0.0):Point<3>(xi,yi,zi)
        ,mWeight(w)
        {

        }
    //}
    
    // @ Destructor {
        ~Integration_Point()
        {

        }
    //}

    // @ Access{
        double& Weight()
        {
            return mWeight;
        }
        double Weight()const
        {
            return mWeight;
        }
    // }
    private:
        double mWeight;
};

#endif