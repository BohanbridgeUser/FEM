#include "Vector.h"

/*

Vector2D Method

*/

/* Constructor */
Vector2D::Vector2D():i(0.0),j(0.0)
{
    
}
Vector2D::Vector2D(const double& xi, const double& yi):i(xi),j(yi)
{

}
/* Copy Constructor */
Vector2D::Vector2D(const Vector2D& another)
{
    i = another.i;
    j = another.j;
}
/* Destructor */
Vector2D::~Vector2D()
{

}

/* Element Access */
double Vector2D::x() const
{
    return i;
}
double Vector2D::y() const
{
    return j;
}
double& Vector2D::x()
{
    return i;
}
double& Vector2D::y()
{
    return j;
}

/* Algebra Operations */
Vector2D Vector2D::operator+(const Vector2D& another)const
{
    return Vector2D(i+another.i,j+another.j);
}
Vector2D Vector2D::operator-(const Vector2D& another)const
{
    return Vector2D(i-another.i,j-another.j);
}
Vector2D Vector2D::operator*(const double& alpha)const
{
    return Vector2D(i*alpha,j*alpha);
}
Vector2D operator+(const Vector2D& a, const Vector2D& b)
{
    return Vector2D(a.x()+b.x(),a.y()+b.y());
}
Vector2D operator-(const Vector2D& a, const Vector2D& b)
{
    return Vector2D(a.x()-b.x(),a.y()-b.y());
}
Vector2D operator*(const double& alpha, const Vector2D& a)
{
    return Vector2D(a.i*alpha,a.j*alpha);
}




/*

Vector3D Method

*/
/* Constructor */
Vector3D::Vector3D():Vector2D(),k(0.0)
{

}
Vector3D::Vector3D(const double& x, const double& y, const double& z)
:Vector2D(x,y),k(z)
{

}
/* Copy Constructor */
Vector3D::Vector3D(const Vector3D& another)
:Vector2D(another.x(),another.y()),k(another.z())
{

}
Vector3D::Vector3D(const Vector2D& a, const double& z)
:Vector2D(a),k(z)
{

}
/* Destructor */
Vector3D::~Vector3D()
{

}

/* Element Access */
double Vector3D::z() const
{
    return k;
}
double& Vector3D::z() 
{
    return k;
}

/* Algebra Operations */
Vector3D Vector3D::operator+(const Vector3D& another) const
{
    return Vector3D(x()+another.x(),y()+another.y(),z()+another.z());
}
Vector3D Vector3D::operator-(const Vector3D& another) const
{
    return Vector3D(x()-another.x(),y()-another.y(),z()-another.z());
}
Vector3D Vector3D::operator*(const double& alpha) const
{
    return Vector3D(x()*alpha,y()*alpha,z()*alpha);
}
Vector3D operator+(const Vector3D& a, const Vector3D& b)
{
    return Vector3D(a.x()+b.x(),a.y()+b.y(),a.z()+b.z());
}
Vector3D operator-(const Vector3D& a, const Vector3D& b)
{
    return Vector3D(a.x()-b.x(),a.y()-b.y(),a.z()-b.z());
}
Vector3D operator*(const double& alpha, const Vector3D& a)
{
    return Vector3D(a.x()*alpha,a.y()*alpha,a.z()*alpha);
}
