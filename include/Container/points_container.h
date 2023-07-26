#ifndef _CONTAINER_H_
#define _CONTAINER_H_
#include <vector>
#include <utility>
#include "../define.h"
template <typename TPointType>
class Points_Container
{
    
    public:
        typedef TPointType PointType;
        typedef std::vector<PointType> PointsVector;

        LOTUS_POINTER_DEFINE(Points_Container<PointType>)

    // @ Constructor {
        /* Default */
        Points_Container():pData()
        {
            number = 0;
        }

        /* Copy Constructor */
        Points_Container(PointsVector& InputPointsVector)
        {
            pData = InputPointsVector;
            number = InputPointsVector.size();
        }
    //}

    // @ Destructor {
        ~Points_Container()
        {
            
        }
    //}

    // @ Access { 
        TPointType& GetValue(const int& index)
        {
            return pData[index];
        }    
        TPointType GetValue(const int& index) const
        {
            return pData[index];
        }
    //}

    // @ Utility { 
        void InsertPoint(const PointType& point)
        {
            pData.push_back(point);
        }
    // }
    /* private member */
    private:
        PointsVector pData;
        int number;
};
#endif