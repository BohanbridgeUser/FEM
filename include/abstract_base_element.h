#ifndef _ABSTRACT_BASE_ELEMENT_H_
#define _ABSTRACT_BASE_ELEMENT_H_
#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <Eigen/Eigen>
class Abstract_base_element{
    public:
        struct Node{
            int id;
            double x,y,z;
            static int number;
        };
        std::vector<Node*> node;
        int etype; 
        std::map<int, double> properties;
        Eigen::MatrixXd* Joccib;
        Eigen::MatrixXd* B;

        Abstract_base_element();
        Abstract_base_element(char* filename, )
        virtual ~Abstract_base_element();
        virtual Eigen::MatrixXd* Joccib_Matrix()=0;
        virtual Eigen::MatrixXd* B_Matrix();
};
#endif