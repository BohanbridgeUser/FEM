#ifndef _SOLVER_H_
#define _SOLVER_H_
#include "element.h"
#include "force.h"
#include "constrain.h"
class Solver{
    public:
        Eigen::VectorXd dis;
        Eigen::VectorXd displacement(Element& ele,Force& f, Constrain& con);
};
#endif