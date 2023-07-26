#ifndef _POINT_H_
#define _POINT_H_
#include "../define.h"
#include "Vector.h"
#include <iostream>
template<int TDimension>
class Point:public Vector<TDimension>
{
    public:

    LOTUS_POINTER_DEFINE(Point<TDimension>)

    // @ Constructor {
        /* Default */
        Point():Vector<TDimension>()
        {
            
        }
        Point(const double& xi, const double& yi):Vector<TDimension>(xi,yi)
        {
            number++;
            ID= number;
        }
        Point(const double& xi, const double& yi, const double& zi):Vector<TDimension>(xi,yi,zi)
        {
            number++;
            ID= number;
        }
        Point(const int& p, const double& xi, const double& yi):Vector<TDimension>(xi,yi), ID(p)
        {
            number++;
        }
        Point(const int& p, const double& xi, const double& yi, const double& zi):Vector<TDimension>(xi,yi,zi),ID(p)
        {
            number++;
        }
        Point(const Point& another):Vector<TDimension>(another.x(),another.y()),ID(another.ID)
        {
            if(TDimension == 3) Vector<TDimension>::z() = another.z();
            number++;
        }
        Point(const int& p, const Vector<TDimension>& v2p):Vector<TDimension>(v2p),ID(p)
        {
            number++;
        }
    //}

    // @ Destructor {
        ~Point()
        {
            if (number > 0) number--; 
        }
    //}

    // @ /* Algebra Operations*/ {
        Point& operator*(const double& alpha)
        {
            std::cout << "Operator* Version 1\n";
            if(TDimension == 2){
                this->coordinate[0]*=alpha;
                this->coordinate[1]*=alpha;
                return *this;
            }
            else {
                this->coordinate[0]*=alpha;
                this->coordinate[1]*=alpha;
                this->coordinate[2]*=alpha;
                return *this;
            }
        }
        Point& operator+(const Point& another)
        {
            std::cout << "Operator+ Version 1\n";
            if(TDimension  == 2){
                this->coordinate[0]+=another.coordinate[0];
                this->coordinate[1]+=another.coordinate[1];
                return *this;
            }
            else {
                this->coordinate[0]+=another.coordinate[0];
                this->coordinate[1]+=another.coordinate[1];
                this->coordinate[2]+=another.coordinate[2];
                return *this;
            }
        }
        Point& operator-(const Point& another)
        {
            std::cout << "Operator- Version 1\n";
            if(TDimension  == 2){
                this->coordinate[0]-=another.coordinate[0];
                this->coordinate[1]-=another.coordinate[1];
                return *this;
            }
            else {
                this->coordinate[0]-=another.coordinate[0];
                this->coordinate[1]-=another.coordinate[1];
                this->coordinate[2]-=another.coordinate[2];
                return *this;
            }
        }
        friend Point<TDimension> operator+(const Point<TDimension>& a, const Point<TDimension>& b)
        {
            if (TDimension  == 2) return  Point<TDimension>( a.coordinate[0]+b.coordinate[0],
                                                                      a.coordinate[1]+b.coordinate[1]);
            else return Point<TDimension>( a.coordinate[0]+b.coordinate[0],
                                               a.coordinate[1]+b.coordinate[1],
                                               a.coordinate[2]+b.coordinate[2]);
        }
        friend Point<TDimension> operator-(const Point<TDimension>& a, const Point<TDimension>& b)
        {
            if (TDimension  == 2) return  Point<TDimension>( a.coordinate[0]-b.coordinate[0],
                                                                      a.coordinate[1]-b.coordinate[1]);
            else return Point<TDimension>( a.coordinate[0]-b.coordinate[0],
                                               a.coordinate[1]-b.coordinate[1],
                                               a.coordinate[2]-b.coordinate[2]);
        }
        friend Point<TDimension> operator*(const double& alpha, const Point<TDimension>& a)
        {
            if (TDimension  == 2) return  Point<TDimension>( a.coordinate[0]*alpha,
                                                                      a.coordinate[1]*alpha);
            else return Point<TDimension>( a.coordinate[0]*alpha,
                                               a.coordinate[1]*alpha,
                                               a.coordinate[2]*alpha);
        }
        friend std::ostream& operator<<(std::ostream& os, const Point& a)
        {
             if (TDimension  == 2) {
                os << a.ID << ' ' << a.x() << ' ' << a.y() << std::endl;
                return os;
            }else {
                os << a.ID << ' ' << a.x() << ' ' << a.y() << ' ' << a.z() << std::endl;
                return os;
            }
        }
    //}
    
    // @ Utility{
        int GetID() const
        {
            return ID;
        }
        friend double dist(const Point& a, const Point& b)
        {
            return Point(b-a).norm();
        }
    // }
    private:
        int ID;
        static int number;
};
template<int TDimension> int Point<TDimension>::number = 0;
#endif
