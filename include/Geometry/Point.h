#ifndef _POINT_H_
#define _POINT_H_
#include "Lotus_Vector.h"
#include <iostream>
#include <array>

template<int TDimension>
class Point:public Lotus_Vector<TDimension>
{
    public:
    /// @name Define
    /// @{ 
        LOTUS_POINTER_DEFINE(Point<TDimension>)
        typedef size_t
                                                    IndexType;
        typedef size_t                              
                                                     SizeType;
        static int D;
    /// @}
    
    /// @name Lift Circle
    /// @{
        Point():Lotus_Vector<TDimension>()
        {
            
        }
        Point(const double& xi, const double& yi):Lotus_Vector<TDimension>(xi,yi)
        {
            number++;
        }
        Point(const double& xi, const double& yi, const double& zi):Lotus_Vector<TDimension>(xi,yi,zi)
        {
            number++;
        }
        Point(const Point& another):Lotus_Vector<TDimension>(another.x(),another.y())
        {
            if(TDimension == 3) Lotus_Vector<TDimension>::z() = another.z();
            number++;
        }
        Point(const Lotus_Vector<TDimension>& v2p):Lotus_Vector<TDimension>(v2p)
        {
            number++;
        }
        ~Point()
        {
            if (number > 0) number--; 
        }
    /// @}

    /// @name Operators
    /// @{
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
                os << a.x() << ' ' << a.y() << std::endl;
                return os;
            }else {
                os << a.x() << ' ' << a.y() << ' ' << a.z() << std::endl;
                return os;
            }
        }
    //}
    
    /// @name Operations
    /// @{
        friend double dist(const Point& a, const Point& b)
        {
            return Point(b-a).norm();
        }
        std::array<double,3> Coordinates()
        {
            return std::array<double,3>(this->x(),this->y(),this->z());
        }
        std::array<double,3> Coordinates() const
        {
            return std::array<double,3>{this->x(),this->y(),this->z()};
        }
    /// @}

    /// @name Access
    /// @{
        
    /// @}
    private:
        static int number;
        
};
template<int TDimension> int Point<TDimension>::number = 0;
template<int TDimension> int Point<TDimension>::D = TDimension;
#endif
