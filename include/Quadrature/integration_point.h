#ifndef _INTEGRATION_POINT_H_
#define _INTEGRATION_POINT_H_
#include "../Geometry/Point.h"

template<int TDimension>
class Integration_Point:public Point<3>
{
    public:
        // @ Constructor {
        /* Default */
        Integration_Point(const double& xi, const double& yi=0.0, const double& zi=0.0, const double& w):Point<TDimension>(xi,yi,zi)
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