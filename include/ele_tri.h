#ifndef _ELE_TRI_H_
#define _ELE_TRI_H_
#include "abstract_base_element.h"
// class Abstract_base_element{
//     public:
//         struct Node{
//             int id;
//             double x,y,z;
//             static int number;
//         };
//         std::vector<Node*> node;
//         int etype; 
//         std::map<std::string, double> properties;

//         Abstract_base_element();
//         virtual ~Abstract_base_element();
//         virtual Eigen::MatrixXd* Joccib_Matrix()=0;
//         virtual Eigen::MatrixXd* B_Matrix();
// };
class Triangle:public Abstract_base_element {
    public:
        Triangle();
        ~Triangle();
        
};
#endif