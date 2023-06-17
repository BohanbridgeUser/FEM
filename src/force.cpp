#include "../include/force.h"
#include <iostream>
#include <fstream>

Force::Force(char* filename, Element& ele_build)
{
    using std::cout;
    using std::endl;
    using std::ios;
    std::fstream file;
    ptf = nullptr;
    pts = nullptr;
    file.open(filename,ios::in);
    if (file.is_open()){
        cout << "Force.txt opened successfully!\n";
        int fsum;
        file >> fsum;
        double x,y,z;
        file >> x >> y >> z >> g;
        ptf = new triangle_f[ele_build.elenum];
        for (int i=0;i<ele_build.elenum;++i) {
            double A = ele_build.pele_t[i].J / 2.0;
            ptf[i].Pf << 0,1,0,1,0,1;
            ptf[i].Pf *= -1.0 / 3.0 * y * g * Element::element_triangle::t * A;
            cout << ptf[i].Pf << endl;
        }
        cout << endl;
        pts = new triangle_s[fsum];
        for (int i=0;i<fsum;++i) {
            file >> pts[i].elenum;
            file >> pts[i].sftype;
            file >> pts[i].node1;
            file >> pts[i].node2;
            file >> pts[i].q;
            double l = (ele_build.pele_t->epnode->pnode[pts[i].node1 -1] - ele_build.pele_t->epnode->pnode[pts[i].node2 -1]).norm();
            switch (pts[i].sftype){
                case 0:
                    pts[i].Ps << 0,1,0,1,0,0;
                    pts[i].Ps *= 1 / 2.0 * pts[i].q * l * ele_build.pele_t->t;
                case 1:
                    pts[i].Ps << 0,1,0,1,0,0;
                    pts[i].Ps *= -1 / 2.0 * pts[i].q * l * ele_build.pele_t->t;
                case 2:
                    pts[i].Ps << 1,0,1,0,0,0;
                    pts[i].Ps *= 1 / 2.0 * pts[i].q * l * ele_build.pele_t->t;
                case 3:
                    pts[i].Ps << 1,0,1,0,0,0;
                    pts[i].Ps *= -1 / 2.0 * pts[i].q * l * ele_build.pele_t->t;
                default:
                    break;
                
            }
            cout << pts[i].Ps << endl;
        }
    }else {
        std::cerr << "Failed open Force.txt\n";
        exit(0);
    }
}
Force::~Force()
{
    delete ptf;
    delete pts;
}