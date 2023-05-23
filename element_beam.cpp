#include "element_beam.h"

int Element_Beam::ecount = 0;

Element_Beam::Element_Beam(Node i_, Node j_, double e_, double h_, double b_)
{
	i = i_, j = j_, E = e_, h = h_, b = b_;
	calI();
	ecount++;
}
Element_Beam::Element_Beam(double xi, double yi, double xj, double yj, double i_, double j_, double e_, double h_, double b_)
{
	i.x() = xi, i.y() = yi, i.z() = i_, j.x() = xj, j.y() = yj, j.z() = j_, E = e_, h = h_, b = b_;
	calI();
	ecount++;
}
Element_Beam::~Element_Beam()
{
	ecount--;
}
double Element_Beam::length()const 
{
	return (j - i).norm();
}
Eigen::Matrix4d Element_Beam::element_stiffness()const
{
	double l = length();
	Eigen::Matrix4d M1{
		{12		,		6*l,	 -12,		6*l},
		{6*l	,4*pow(l,2),	-6*l,2*pow(l,2)},
		{-12	,	   -6*l,	  12,	   -6*l},
		{6*l	,2*pow(l,2),	-6*l,4*pow(l,2)},
	};
	return (E * I / pow(l, 3) * M1);
}