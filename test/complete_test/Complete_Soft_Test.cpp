#include "../../include/Model/model.h"
#include "../../include/Element/solid_element.h"
#include "../../include/Condition/point_load_condition.h"
#include "../../include/Variable/variable.h"
#include "../../include/Solve_Strategy/builder_and_solver/block_builder_and_solver.h"
#include "../../include/Solve_Strategy/strategies/linear_solving_strategy.h"
#include "../../include/Solve_Strategy/convergencecriterias/convergencecriterias.h"
#include "../../include/Solve_Strategy/schemes/scheme.h"
#include "../../include/Container/lotus_flags.h"
int main()
{
    /* Geometry Model */
    std::cout << "********************Geometry Model********************\n";
    std::vector<Point<3> > V_PP;
    for (int i=0;i<9;++i)
    {
        for (int j=0;j<9;++j)
        {
            for(int k=0;k<9;++k)
            {
                Point<3> *pP = new Point<3>(1.00*i,1.00*j,1.00*k);
                V_PP.push_back(*pP);
            }
        }
    }
    
    for (auto iterator=V_PP.begin();iterator!=V_PP.end();++iterator)
        std::cout << "Point " << iterator-V_PP.begin() << " " << *iterator;
    
    Hexahedron<Point<3> >* H_3d_8s[8*8*8];
    int cnt = 0;
    for (int i=0;i<8;++i) 
    {
        for (int j=0;j<8;++j) 
        {
            for(int k=0;k<8;++k)
            {
                H_3d_8s[cnt] = new Hexahedron(cnt,V_PP[i*81+j*9+k],V_PP[(i+1)*81+j*9+k],V_PP[(i+1)*81+(j+1)*9+k],V_PP[i*81+(j+1)*9+k],
                                                V_PP[i*81+j*9+(k+1)],V_PP[(i+1)*81+j*9+(k+1)],V_PP[(i+1)*81+(j+1)*9+(k+1)],V_PP[i*81+(j+1)*9+(k+1)]);
                cnt++;
            }
        }
    }
    typedef Hexahedron<Point<3> > Geomelem;
    Geometry_Container<Geomelem> G_C;
    for(int i=0;i<8*8*8;++i) 
    {
        G_C.Insert(*H_3d_8s[i]);
        std::cout << G_C.At(i);
    }
    // /* Mesh Model */
    // std::cout << "********************Mesh Model********************\n";
    // Node Ns[81];
    // for (int i=0;i<81;++i)
    // {
    //     Ns[i] = Node(P[i]);
    // }
    // for (int i=0;i<81;++i)
    // {
    //     std::cout << Ns[i];
    // }    
    // std::vector<Node> N_V;
    // for (int i=0;i<81;++i)
    // {
    //     N_V.push_back(Ns[i]);
    // }
    // for (int i=0;i<81;++i)
    // {
    //     std::cout << N_V[i];
    // }
    // Quadrilateral_3d_4<Node > Q_3d_4Ns[64];
    // for (int i=0;i<8;++i) 
    // {
    //     for (int j=0;j<8;++j) 
    //     {
    //         Q_3d_4Ns[i*8+j] = Quadrilateral_3d_4<Node >(N_V[i*8+j],N_V[i*8+j+1],N_V[(i+1)*8+j],N_V[(i+1)*8+j+1]);
    //     }
    // }
    // Properties Prop;
    // std::vector<double> V_E,V_M,V_P;
    // V_E.push_back(2.1E9);
    // V_M.push_back(7850.00);
    // V_P.push_back(1.2E-5);

    // std::vector<Solid_Element> E_V;
    // for (int i=0;i<64;++i) 
    // {
    //     E_V.push_back(Solid_Element(i,&Q_3d_4Ns[i],&Prop));
    // }

    // Condition Cond;
    // std::vector<Condition> C_V;
    // C_V.push_back(Cond);
    // Mesh<Node,Properties,Solid_Element,Condition> 
    //                     mesh(&N_V,Prop,E_V,C_V);
    // for(int i=0;i<mesh.Elements().size();++i)
    // {
    //     std::cout << "Element ID: " << i+1 << std::endl;
    //     std::cout << mesh.Elements()[i];
    // }
    return 0;
}