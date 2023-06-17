#ifndef _FORCE_H_
#define _FORCE_H_
#include "node.h"
#include "element.h"
#include <memory>
class Force{
    public:
       struct triangle_f
       {
            Eigen::Vector<double,6> Pf;
       };
       struct triangle_s
       {
            int elenum;
            int node1,node2;
            int sftype;
            double q;
            Eigen::Vector<double,6> Ps;
       };

       double rou;
       double g;
       triangle_f* ptf;
       triangle_s* pts;
       Force(char* filename, Element& ele_build);
       ~Force();
};

#endif