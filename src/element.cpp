#include "../include/element.h"

double Element::element_triangle::D0 = 0.0;
double Element::element_triangle::t = 0.0;
Eigen::Matrix3d Element::element_triangle::D = Eigen::Matrix<double,3,3>::Zero();
Eigen::MatrixXd Element::element_triangle::B = Eigen::Matrix<double,3,6>::Zero();
int Element::element_triangle::stressorstrain = 0;
Node* Element::element_triangle::epnode = nullptr;

double Element::element_triangle::area() 
{   
    double a = 0.0;
    Node::node temp{0,0,0,0};
    temp = temp.crossproduct(epnode->pnode[Node1],epnode->pnode[Node2]);
    temp = temp + temp.crossproduct(epnode->pnode[Node2],epnode->pnode[Node3]);
    temp = temp + temp.crossproduct(epnode->pnode[Node3],epnode->pnode[Node1]);
    a = temp.norm();
    return a;
}
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
            int etype;
            file >> etype;
            switch (etype){
                case 1:
                    etype = 1;
                    pele_b = new element_beam[elenum];
                    for (int i=0;i<elenum;++i) {
                        file >> pele_b[i].enumber;
                        file >> pele_b[i].Node1;
                        file >> pele_b[i].Node2;
                        cout << "Element: ";
                        cout << pele_b[i].enumber << endl;
                        cout << pele_b[i].Node1 << endl;
                        cout << pele_b[i].Node2 << endl;
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
                        file >> pele_t[i].Node1;
                        file >> pele_t[i].Node2;
                        file >> pele_t[i].Node3;
                        cout << "\nElement: ";
                        cout << pele_t[i].enumber << ' ';
                        cout << pele_t[i].Node1 << ' ';
                        cout << pele_t[i].Node2 << ' ';
                        cout << pele_t[i].Node3 << endl;
                        pele_t[i].J_set();
                        pele_t[i].Ke_set();
                    }
                    break;
                case 3:
                    etype = 3;
                    pele_q = new element_quadrangle[elenum];
                    for (int i=0;i<elenum;++i) {
                        file >> pele_q[i].enumber;
                        file >> pele_q[i].Node1;
                        file >> pele_q[i].Node2;
                        file >> pele_q[i].Node3;
                        file >> pele_q[i].Node4;
                        cout << "Element: " ;
                        cout << pele_q[i].enumber << endl;
                        cout << pele_q[i].Node1 << endl;
                        cout << pele_q[i].Node2 << endl;
                        cout << pele_q[i].Node3 << endl;
                        cout << pele_q[i].Node4 << endl;
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
    r << epnode->pnode[Node1-1].x,epnode->pnode[Node1-1].y,
         epnode->pnode[Node2-1].x,epnode->pnode[Node2-1].y,
         epnode->pnode[Node3-1].x,epnode->pnode[Node3-1].y;
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