#include <iostream>
#include <eigen3/Eigen/Eigen>
#include <cmath>
#include "element_beam.h"
#include <fstream>

Eigen::MatrixXd assemble_stifness(const std::vector<Element_Beam*> elements)
{
	int size = 2 * (elements.back()->showcount() + 1);
	std::cout << size << std::endl;
	Eigen::MatrixXd return_Matrix(size, size);
	return_Matrix.setZero(size , size);
	for (auto i = elements.begin(); i != elements.end(); ++i) {
		int ni = (*i)->inum();
		int nj = (*i)->jnum();
		std::cout << "ni:" << ni << '\t' << "nj:" << nj << std::endl;
		Eigen::Matrix4d elem_stif = (*i)->element_stiffness();
		std::cout << elem_stif << std::endl;
		for (int k = 0; k < 4; ++k) {
			for (int l = 0; l < 4; ++l) {
				return_Matrix(2 * ni + k, 2 * ni + l) += elem_stif(0 + k, 0 + l);
			}
		}
	}
	return return_Matrix;
}


int main()
{
	/* 
	TL   : the total length of beam
	   L    : the length of element
	ele_num : the number of element
	*/
	double TL = 500.0, L = 10.0;
	int ele_num = (TL / L);
	/* 
	origin is (0,0), create the beam 
	E = 210e3
	h = 2
	b = 100
	*/
	
	/* assemble stifness matrix */
	double E = 210000, h = 2, b = 100;
	std::vector<Element_Beam*> elements;
	for (int i = 0; i < ele_num; ++i) {
		Element_Beam*  newelement = new Element_Beam(i, 0, i + L, 0, i, i + 1, E, h, b);
		elements.push_back(newelement);
	}
	Eigen::MatrixXd assem_stifness = assemble_stifness(elements);
	std::cout << std::endl << assem_stifness.rows() << std::endl;
	//std::cout << "Stiffness_Matrix:\n " << assem_stifness << std::endl;
	
	/* constrain penalty function method */
	/* this question constrain i = 0 and j = 1000 */
	double alpha = E * (1.0 / 12.0 * pow(h, 3) * b) * 100000 /pow(L,3);
	assem_stifness(0, 0) += alpha;
	assem_stifness(1, 1) += alpha;
	assem_stifness((ele_num + 1) * 2 - 2, (ele_num + 1) * 2 - 2) += alpha;
	assem_stifness((ele_num + 1) * 2 - 1, (ele_num + 1) * 2 - 1) += alpha;
	Eigen::VectorXd F((ele_num + 1) * 2 - 4);
	F.setZero((ele_num + 1) * 2 - 4);
	F(ele_num) = -400;
	auto inverse_stifness = assem_stifness.inverse();
	Eigen::VectorXd U((ele_num + 1) * 2 - 4);
	U = inverse_stifness * F;

	/* direct elimination */
	//Eigen::MatrixXd new_stifness = assem_stifness.block(2, 2, (ele_num+1)*2-4, (ele_num + 1) * 2 -4);
	////std::cout << new_stifness << std::endl;
	///* Load the concentrate force */
	//Eigen::VectorXd F((ele_num + 1) * 2 - 4);
	//F.setZero((ele_num + 1) * 2 - 4);
	//F(ele_num-2) = -400;

	///* solve the displacement */
	//Eigen::VectorXd U((ele_num + 1) * 2 - 4);
	//auto inverse_stifness = new_stifness.inverse();
	////std::cout << "inverse_Matrix : " << inverse_stifness << std::endl;
	////std::cout << inverse_stifness.size();
	//U = inverse_stifness * F;

	/* output the result */
	std::cout << "U: " << U << std::endl;
	std::cout << "Midle point U: " << U(ele_num) << std::endl;
	return 0;
}
