#include "include/node.h"
#include "include/element.h"
#include "include/force.h"
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
        std::cout << "\nForcec File : " << argv[3] << std::endl;
        Force force(argv[3],element);
    }
    return 0;
}