#include "../include/force.h"
#include <iostream>
#include <fstream>
int Force::triangle_s::fsum = 0;

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
        file >> triangle_s::fsum;
        double x,y,z;
        file >> x >> y >> z >> g;
        ptf = new triangle_f[ele_build.elenum];
        for (int i=0;i<ele_build.elenum;++i) {
            double A = ele_build.pele_t[i].J / 2.0;
            ptf[i].Pf << 0,1,0,1,0,1;
            ptf[i].Pf *= -1.0 / 3.0 * y * g * Element::element_triangle::t * A;
            cout << "Pf Input: "<< endl;
            cout << ptf[i].Pf << endl;
        }
        cout << endl;
        pts = new triangle_s[triangle_s::fsum];
        for (int i=0;i<triangle_s::fsum;++i) {
            file >> pts[i].elenum;
            pts[i].elenum--;
            file >> pts[i].sftype;
            file >> pts[i].node1;
            pts[i].node1--;
            file >> pts[i].node2;
            pts[i].node2--;
            file >> pts[i].q;
            double l = (ele_build.pele_t->epnode->pnode[pts[i].node1 -1] - ele_build.pele_t->epnode->pnode[pts[i].node2 -1]).norm();
            std::cout << pts[i].sftype<< std::endl;
            switch (pts[i].sftype){
                case 0:
                    pts[i].Ps << 0,1,0,1,0,0;
                    pts[i].Ps *= 1 / 2.0 * pts[i].q * l * ele_build.pele_t->t;
                    break;                
                case 1:
                    pts[i].Ps << 0,1,0,1,0,0;
                    //std::cout << pts[i].Ps << std::endl;
                    pts[i].Ps *= -1 / 2.0 * pts[i].q * l * ele_build.pele_t->t;
                    //std::cout << pts[i].Ps << std::endl;
                    break;
                case 2:
                    pts[i].Ps << 1,0,1,0,0,0;
                    pts[i].Ps *= 1 / 2.0 * pts[i].q * l * ele_build.pele_t->t;
                    break;
                case 3:
                    pts[i].Ps << 1,0,1,0,0,0;
                    pts[i].Ps *= -1 / 2.0 * pts[i].q * l * ele_build.pele_t->t;
                    break;
                default:
                    break;
            }
            cout << "Ps Input: \n";
            cout << pts[i].Ps << endl;
        }
    }else {
        std::cerr << "Failed open Force.txt\n";
        exit(0);
    }
}
Force::~Force()
{
    delete[] ptf;
    delete[] pts;
}
Eigen::VectorXd Force::assemble_force(Element& ele_build)
{
    assemble_f.resize(2 * ele_build.nodenum);
    for (int i=0;i<ele_build.elenum;++i) {
        for (int j=0;j<3;++j) {
            assemble_f((ele_build.pele_t[i].NodeN[j])*2) 
                += ptf[i].Pf(j*2);
            assemble_f((ele_build.pele_t[i].NodeN[j])*2+1) 
                += ptf[i].Pf(j*2+1);
        }
    }
    for (int i=0;i<triangle_s::fsum;++i) {
        for (int j=0;j<3;++j) {
            assemble_f((ele_build.pele_t[pts[i].elenum].NodeN[j])*2) 
                += pts[i].Ps(j*2);
            assemble_f((ele_build.pele_t[pts[i].elenum].NodeN[j])*2+1) 
                += pts[i].Ps(j*2+1);
        }
    }
    std::cout << "\nAssemble_force: " <<std::endl;
    std::cout << assemble_f << std::endl;
    std::cout << "sum: " << assemble_f.sum() << std::endl;
    return assemble_f;
}