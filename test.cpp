#include "include/node.h"
#include "include/element.h"
#include "include/force.h"
#include "include/constrain.h"
#include "include/solver.h"
#include <iostream>
int main(int argc, char* argv[])
{
    std::cout << argc << std::endl;
    if (argc > 1) {
        std::cout << "Program Running: " << argv[0] << std::endl;
        std::cout << "Node File : " << argv[1] << std::endl;
        Node node(argv[1]);
        std::cout << "\nElement File : " << argv[2] << std::endl;
        Element element(argv[2],&node);
        element.assemble_stiffness();
        std::cout << "\nForcec File : " << argv[3] << std::endl;
        Force force(argv[3],element);
        force.assemble_force(element);
        Constrain con(argv[4]);
        Solver solve;
        solve.displacement(element,force,con);
    }
    return 0;
}