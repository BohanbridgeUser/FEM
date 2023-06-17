#ifndef _CONSTRAIN_H_
#define _CONSTRAIN_H_
#include "../include/element.h"
class Constrain{
        friend class solver;
    public:
        int* Con_Node;
        double* Con_value1;
        double* Con_value2;
        int csum;
        int ctype;
        Constrain(char* filename);
        ~Constrain();
};
#endif