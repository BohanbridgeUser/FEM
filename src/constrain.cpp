#include "../include/constrain.h"

Constrain::Constrain(char* filename)
{
    using std::cout;
    using std::endl;
    using std::ios;
    std::fstream file;
    Con_Node = nullptr;
    Con_value1 = nullptr;
    Con_value2 = nullptr;
    file.open(filename,ios::in);
    if (file.is_open()) {
        file >> csum;
        Con_Node = new int[csum];
        Con_value1 = new double[csum];
        Con_value2 = new double[csum];
        file >> ctype;
        for (int i=0;i<csum;++i) {
            file >> Con_Node[i];
            Con_Node[i]--;
            file >> Con_value1[i];
            file >> Con_value2[i];
            std::cout << "Node: " << Con_Node[i] <<' ' << Con_value1[i] << ' '
             << Con_value2[i] << std::endl;
        }
    }else{
        std::cerr << "Constrain.txt open failed!\n";
        exit(0);
    }
}
Constrain::~Constrain()
{
    delete[] Con_Node;
    delete[] Con_value1;
    delete[] Con_value2; 
}