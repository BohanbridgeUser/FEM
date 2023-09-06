#ifndef _LOTUS_VECTOR_H_
#define _LOTUS_VECTOR_H_
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "../define.h"
#include "geometry_dimension.h"

// @ Vector class is defined in this file
//   
//   

template<int TDimension>
class Lotus_Vector
{
    protected:
        double coordinate[TDimension];
    public:
        // @ Define { 
        LOTUS_POINTER_DEFINE(Lotus_Vector<TDimension>)
        // }

        // @ Constructor {
        Lotus_Vector()
        {
            for(int i=0;i<TDimension;++i) 
                coordinate[i] = 0;
        }   
        Lotus_Vector(const double& xi, const double& yi)
        {
            coordinate[0] = xi; coordinate[1] = yi;
            if (TDimension == 3) coordinate[2] = 0.0;
        }
        Lotus_Vector(const double& xi, const double& yi, const double& zi)
        {
            coordinate[0] = xi; coordinate[1] = yi; coordinate[2] = zi;
        }

        /* Copy Constructor */
        Lotus_Vector(const Lotus_Vector& another)
        {
            for (int i=0;i<TDimension;++i) coordinate[i] = another.coordinate[i];
        }
        //}

        // @ Destructor {
        ~Lotus_Vector()
        {

        }
        //}

        
        // @ Algebra Operations {
        Lotus_Vector& operator+(const Lotus_Vector& another)
        {
            if(TDimension == 2){
                coordinate[0]+=another.coordinate[0];
                coordinate[1]+=another.coordinate[1];
                return *this;
            }
            else {
                coordinate[0]+=another.coordinate[0];
                coordinate[1]+=another.coordinate[1];
                coordinate[2]+=another.coordinate[2];
                return *this;
            }
        }
        Lotus_Vector& operator-(const Lotus_Vector& another)
        {
            if(TDimension == 2){
                coordinate[0]-=another.coordinate[0];
                coordinate[1]-=another.coordinate[1];
                return *this;
            }
            else {
                coordinate[0]-=another.coordinate[0];
                coordinate[1]-=another.coordinate[1];
                coordinate[2]-=another.coordinate[2];
                return *this;
            }
        }
        Lotus_Vector& operator*(const double& alpha)
        {
            if(TDimension == 2){
                coordinate[0]*=alpha;
                coordinate[1]*=alpha;
                return *this;
            }
            else {
                coordinate[0]*=alpha;
                coordinate[1]*=alpha;
                coordinate[2]*=alpha;
                return *this;
            }
        }
        friend Lotus_Vector operator+(const Lotus_Vector& a, const Lotus_Vector& b)
        {
            if (TDimension == 2) return  Lotus_Vector<TDimension>(a.coordinate[0]+b.coordinate[0],
                                                                     a.coordinate[1]+b.coordinate[1]);
            else return Lotus_Vector<TDimension>(a.coordinate[0]+b.coordinate[0],
                                              a.coordinate[1]+b.coordinate[1],
                                              a.coordinate[2]+b.coordinate[2]);
        }
        friend Lotus_Vector<TDimension> operator-(const Lotus_Vector<TDimension>& a, const Lotus_Vector<TDimension>& b)
        {
            if (TDimension == 2) return  Lotus_Vector<TDimension>(a.coordinate[0]-b.coordinate[0],
                                                                     a.coordinate[1]-b.coordinate[1]);
            else return Lotus_Vector<TDimension>(a.coordinate[0]-b.coordinate[0],
                                              a.oordinate[1]-b.coordinate[1],
                                              a.coordinate[2]-b.coordinate[2]);
        }
        friend Lotus_Vector<TDimension> operator*(const double& alpha, const Lotus_Vector<TDimension>& a)
        {
            if (TDimension == 2) return  Lotus_Vector<TDimension>(a.coordinate[0]*alpha,
                                                                     a.coordinate[1]*alpha);
            else return Lotus_Vector<TDimension>(a.coordinate[0]*alpha,
                                              a.coordinate[1]*alpha,
                                              a.coordinate[2]*alpha);
        }
        double norm() const
        {
            std::cout << *this;
            return sqrt(coordinate[0]*coordinate[0]+
                        coordinate[1]*coordinate[1]+
                        coordinate[2]*coordinate[2]);
        }
        double norm2() const
        {
            return coordinate[0]*coordinate[0]+
                        coordinate[1]*coordinate[1]+
                        coordinate[2]*coordinate[2];
        }
        // }

        // @ Utility {
        friend std::ostream& operator<<(std::ostream& os, const Lotus_Vector& a)
        {
            if (TDimension == 2) {
                os << a.x() << ' ' << a.y() << std::endl;
                return os;
            }else {
                os << a.x() << ' ' << a.y() << ' ' << a.z() << std::endl;
                return os;
            }
        }
        double x() const
        {
            return coordinate[0];
        }
        double y() const
        {
            return coordinate[1];
        }
        double z() const
        {
            if (TDimension == 3) return coordinate[2];
            else return 0.0;
        }
        double& x()
        {
            return coordinate[0];
        }
        double& y()
        {
            return coordinate[1];
        }
        double& z()
        {
            if (TDimension == 3) return coordinate[2];
            else exit(0);
        }
        // }
};
//}
#endif