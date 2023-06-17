#include "solver.h"

Eigen::VectorXd Solver::displacement(Element& ele, Force& f, Constrain& con)
{
    dis.resize(2 * ele.nodenum);
    double a = 1e5;
    for (int i=0;i<con.csum;++i) {
        ele.stiffness(con.Con_Node[i]*2,con.Con_Node[i]*2) *= (a * con.Con_value1[i]);
        ele.stiffness(con.Con_Node[i]*2,con.Con_Node[i]*2+1) *= (a * con.Con_value2[i]);
        f.assemble_f(con.Con_Node[i]*2) *= (a * con.Con_value1[i]);
        f.assemble_f(con.Con_Node[i]*2+1) *= (a * con.Con_value2[i]);
    }
    std::cout << "test\n";
    dis = ele.stiffness.inverse() * f.assemble_f;
    std::cout << dis <<std::endl;
    return dis;
}
