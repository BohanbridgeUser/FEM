#include "../include/element.h"

double Element::element_triangle::D0 = 0.0;
double Element::element_triangle::t = 0.0;
Eigen::Matrix3d Element::element_triangle::D = Eigen::Matrix<double,3,3>::Zero();
Eigen::MatrixXd Element::element_triangle::B = Eigen::Matrix<double,3,6>::Zero();
int Element::element_triangle::stressorstrain = 0;
Node* Element::element_triangle::epnode = nullptr;

Element::Element(char* filename,Node* pn)
{
    using std::cout;
    using std::cin;
    using std::endl;
    using std::ios;
    std::fstream file;
    file.open(filename,ios::in);
    nodenum = Node::sum;
    if (file.is_open()) {
        cout << "Element.txt Opened successfully!\n";
        pele_t = nullptr;
        pele_q = nullptr;
        pele_b = nullptr;
        file >> elenum;
        if (elenum <= 0) {
            std::cerr << "Element Number Wrong\n";
            exit(0);
        } else {
            file >> etype;
            switch (etype){
                case 1:
                    etype = 1;
                    pele_b = new element_beam[elenum];
                    for (int i=0;i<elenum;++i) {
                        file >> pele_b[i].enumber;
                        pele_b[i].NodeN = new int[2];
                        for (int j=0;j<2;++j) {
                            file >> pele_b[i].NodeN[j];
                            pele_b[i].NodeN[j]--;
                        }
                        cout << "Element: ";
                        cout << pele_b[i].enumber << endl;
                        for (int j=0;j<2;++j) cout <<  pele_b[i].NodeN[j] << endl;
                    }
                    break;
                case 2:
                    etype = 2;
                    pele_t = new element_triangle[elenum];
                    file >> E;
                    file >> v;
                    file >> pele_t->t;
                    file >> pele_t->stressorstrain;
                    pele_t->epnode = pn;
                    pele_t->B_set();
                    if (pele_t->stressorstrain == 0) {
                        pele_t->D0 = E / (1-v);
                    } else {
                        E = E / (1 - v * v);
                        v = v / (1 - v);
                        pele_t->D0 = E / (1 - v * v);
                    }
                    pele_t->D_set(v);
                    pele_t->B_set();
                    for (int i=0;i<elenum;++i) {
                        file >> pele_t[i].enumber;
                        pele_t[i].NodeN = new int[3];
                        for (int j=0;j<3;++j) {
                            file >> pele_t[i].NodeN[j];
                            pele_t[i].NodeN[j]--;
                        }
                        cout << "\nElement: ";
                        cout << pele_t[i].enumber << ' ';
                        for (int j=0;j<3;++j) cout << pele_t[i].NodeN[j] << endl;
                        pele_t[i].J_set();
                        pele_t[i].Ke_set();
                    }
                    break;
                case 3:
                    etype = 3;
                    pele_q = new element_quadrangle[elenum];
                    for (int i=0;i<elenum;++i) {
                        file >> pele_q[i].enumber;
                        pele_q[i].NodeN = new int[4];
                        for (int j=0;j<4;++j) {
                            file >> pele_q[i].NodeN[j];
                            pele_q[i].NodeN[j]--;
                        }
                        cout << "Element: " ;
                        cout << pele_q[i].enumber << endl;
                        for (int j=0;j<4;++j) cout << pele_q[i].NodeN[j] << endl;
                    }
                    break;
                default:
                    break;
            }
        }
    }else {
        std::cerr << "Failed open Element.txt\n";
        exit(0);
    }
}
Element::~Element()
{
    if (pele_q != nullptr) delete[] pele_q;
    if (pele_t != nullptr) delete[] pele_t;
    if (pele_b != nullptr) delete[] pele_b;
}
bool Element::element_triangle::D_set(double v)
{
    D << 1,v,0,
        v,1,0,
        0,0,(1-v)/2.0;
    D = D0 * D;
    return true;
}
bool Element::element_triangle::B_set()
{
    double bi,bj,bm,ci,cj,cm;
    double xi = 0, yi = 0, zi = 0, xj = 1.0, yj = 0, zj = 0, xm = 0, ym = 1.0 , zm = 0;
    bi = yj - ym;
    bj = ym - yi;
    bm = yi - yj;
    ci = -xj + xm;
    cj = -xm + xi;
    cm = -xi + xj;
    B.resize(3,6);
    B << bi,0,bj,0,bm,0,
         0,ci,0,cj,0,cm,
         ci,bi,cj,bj,cm,bm;
    B = B * 1 / (2 * 0.5);
    return true;
}
bool Element::element_triangle::J_set()
{
    double bi,bj,bm,ci,cj,cm;
    double xi = 0, yi = 0, zi = 0, xj = 1.0, yj = 0, zj = 0, xm = 0, ym = 1.0 , zm = 0;
    bi = yj - ym;
    bj = ym - yi;
    bm = yi - yj;
    ci = -xj + xm;
    cj = -xm + xi;
    cm = -xi + xj;
    Eigen::MatrixXd t(2,3);
    t << bi,bj,bm,
         ci,cj,cm;
    Eigen::MatrixXd r(3,2);
    r << epnode->pnode[NodeN[0]].x,epnode->pnode[NodeN[0]].y,
         epnode->pnode[NodeN[1]].x,epnode->pnode[NodeN[1]].y,
         epnode->pnode[NodeN[2]].x,epnode->pnode[NodeN[2]].y;
    J = fabs((t * r).determinant());
    return true;
}
bool Element::element_triangle::Ke_set()
{
    Ke.resize(3,3);
    Ke = B.transpose() * D * B * t * J / 2.0;
    std::cout << Ke << std::endl;
    return true;
}
Eigen::MatrixXd Element::assemble_stiffness()
{
    switch (etype) {
        case 1:
            break;
        case 2:
            std::cout << "2\n";
            stiffness.resize(2*nodenum,2*nodenum);
            for (int i=0;i<elenum;++i) {
                for (int j=0;j<3;++j) {
                    for (int k=0;k<3;k++) {
                        stiffness((pele_t[i].NodeN[j])*2,pele_t[i].NodeN[k]*2)
                            += pele_t[i].Ke(j*2,k*2);
                        stiffness((pele_t[i].NodeN[j])*2+1,pele_t[i].NodeN[k]*2+1)
                            += pele_t[i].Ke(j*2+1,k*2+1);
                    }
                }
            }
            std::cout << "Matrix of stiffness: " << std::endl;
            std::cout << stiffness << std::endl;
            break;
        case 3:
            break;
        default:
            break;
    }
    return stiffness;
}