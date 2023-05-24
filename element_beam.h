#pragma once
#ifndef _ELEMENT_BEAM_H_
#define _ELEMENT_BEAM_H_
#include <iostream>
#include <eigen3/Eigen/Eigen>
#include <cmath>
class Element_Beam 
{
	private:
		static int ecount;
		using Node = Eigen::Vector3d;
		Node i;
		Node j;
		double E;
		double I;
		double h;
		double b;
		void calI() { I = (1.0 / 12.0 * pow(h, 3) * b); }
	public:
		Element_Beam(Node i, Node j, double e_ = 0, double h_ = 0.0, double b_ = 0.0);
		Element_Beam(double xi, double yi, double xj, double yj, double i_, double j_, double e_ = 0.0, double h_ = 0.0, double b_ = 0.0);
		~Element_Beam();
		void set_E(double e_) { E = e_; }
		void set_I(double h_, double b_) { h = h_, b = b_; calI(); }
		double length() const;
		Eigen::Matrix4d element_stiffness() const;
		static int showcount() { return ecount; }
		int inum()const;
		int jnum()const;
};

#endif