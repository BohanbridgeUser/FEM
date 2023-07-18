#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <cstdlib>
#include <cmath>
class Vector2D
{
    private:
        double i,j;
    public:
        /* Constructor */
        Vector2D();
        Vector2D(const double& xi, const double& yi);

        /* Copy Constructor */
        Vector2D(const Vector2D& another);
        /* Destructor */
        ~Vector2D();

        /* Element Access */
        double x() const;
        double y() const;
        double& x();
        double& y();

        /* Algebra Operations*/
        Vector2D operator+(const Vector2D& another) const;
        Vector2D operator-(const Vector2D& another) const;
        Vector2D operator*(const double& alpha) const;
        friend Vector2D operator+(const Vector2D& a, const Vector2D& b);
        friend Vector2D operator-(const Vector2D& a, const Vector2D& b);
        friend Vector2D operator*(const double& alpha, const Vector2D& a);
};
class Vector3D:public Vector2D
{
    private:
        double k;
    public:
        /* Constructor */
        Vector3D();
        Vector3D(const double& x, const double& y, const double& z);

        /* Copy Constructor */
        Vector3D(const Vector3D& another);
        Vector3D(const Vector2D& a, const double& z);

        /* Destructor*/
        ~Vector3D();

        /* Element Access */
        double z() const;
        double& z();

        /* Algebra Operations */
        Vector3D operator+(const Vector3D& another) const;
        Vector3D operator-(const Vector3D& another) const;
        Vector3D operator*(const double& alpha) const;
        friend Vector3D operator+(const Vector3D& a, const Vector3D& b);
        friend Vector3D operator-(const Vector3D& a, const Vector3D& b);
        friend Vector3D operator*(const double& alpha, const Vector3D& a);   
};
#endif