#include "../../include/Model/model.h"
#include "../../include/Element/small_displacement_element.h"
#include "../../include/Condition/point_load_condition.h"
#include "../../include/Variable/variable.h"
#include "../../include/Solve_Strategy/builder_and_solver/block_builder_and_solver.h"
#include "../../include/Solve_Strategy/strategies/linear_solving_strategy.h"
#include "../../include/Solve_Strategy/convergencecriterias/convergencecriterias.h"
#include "../../include/Solve_Strategy/schemes/scheme.h"
#include "../../include/Container/lotus_flags.h"
#include "../../include/process_info.h"
int main()
{
    /* Geometry Model */
    std::cout << "********************Geometry Model********************\n";
    std::vector<Point<3>* > V_PP;
    for (int i=0;i<9;++i)
    {
        for (int j=0;j<9;++j)
        {
            for(int k=0;k<9;++k)
            {
                Point<3> *pP = new Point<3>(1.00*i,1.00*j,1.00*k);
                V_PP.push_back(pP);
            }
        }
    }
    
    // for (auto iterator=V_PP.begin();iterator!=V_PP.end();++iterator)
    //     std::cout << "Point " << iterator-V_PP.begin() << " " << *iterator;
    
    Hexahedron<Point<3> >* H_3d_8s[8*8*8];
    int cnt = 0;
    for (int i=0;i<8;++i) 
    {
        for (int j=0;j<8;++j) 
        {
            for(int k=0;k<8;++k)
            {
                H_3d_8s[cnt] = new Hexahedron(cnt,*V_PP[i*81+j*9+k],*V_PP[(i+1)*81+j*9+k],*V_PP[(i+1)*81+(j+1)*9+k],*V_PP[i*81+(j+1)*9+k],
                                                *V_PP[i*81+j*9+(k+1)],*V_PP[(i+1)*81+j*9+(k+1)],*V_PP[(i+1)*81+(j+1)*9+(k+1)],*V_PP[i*81+(j+1)*9+(k+1)]);
                cnt++;
            }
        }
    }
    typedef Hexahedron<Point<3> > Geomelem;
    GeometryContainer<Geomelem> G_C;
    for(int i=0;i<cnt;++i) 
    {
        G_C.AddGeometry(H_3d_8s[i]);

    }
    /* Mesh Model */
    std::cout << "********************Mesh Model********************\n";
    Mesh<Node,Properties,Element,Condition> mesh;
    std::vector<Node*> V_P_N;
    for(int i=0;i<cnt;++i)
    {
        Node* N_P = new Node(i,*V_PP[i]);
        mesh.AddNode(Node::Pointer(N_P));
    }
    Properties Prop;
    Prop.Id() = 1;
    Prop.SetValue(DENSITY,7850.00);
    Prop.SetValue(ELASTICITY_MODULUS,2.1E9);
    Prop.SetValue(POISON,0.31);
    std::cout << "Property DENSITY           : " << Prop.GetValue(DENSITY) << std::endl;
    std::cout << "Property ELASTICITY_MODULUS: " << Prop.GetValue(ELASTICITY_MODULUS)<< std::endl;
    std::cout << "Property POISON            : " << Prop.GetValue(POISON)<< std::endl;
    mesh.AddProperties(Properties::Pointer(&Prop));
    
    // Hexahedron<Node >* H_3d_8Ns[cnt];
    // cnt =0 ;
    // for (int i=0;i<8;++i) 
    // {
    //     for (int j=0;j<8;++j) 
    //     {
    //         for(int k=0;k<8;++k)
    //         {
    //             H_3d_8Ns[cnt] = new Hexahedron(cnt,V_Node[i*81+j*9+k],V_Node[(i+1)*81+j*9+k],V_Node[(i+1)*81+(j+1)*9+k],V_Node[i*81+(j+1)*9+k],
    //                                             V_Node[i*81+j*9+(k+1)],V_Node[(i+1)*81+j*9+(k+1)],V_Node[(i+1)*81+(j+1)*9+(k+1)],V_Node[i*81+(j+1)*9+(k+1)]);
    //             cnt++;
    //         }
    //     }
    // }
    

    // std::vector<Small_Displacement_Element> E_V;
    // for (int i=0;i<64;++i) 
    // {
    //     E_V.push_back(Small_Displacement_Element(i,Geometry<Node>::Pointer(H_3d_8Ns[i]),Properties::Pointer(&Prop)));
    // }


    // /******************************Condition*********************************/
    // using Mesh_This = Mesh<Node,Properties,Small_Displacement_Element,Point_Load_Condition>; 
    // Mesh_This mesh;
    // for(int i=8;i<729;i+=9)
    // {
    //     mesh.AddNode(Node::Pointer(&V_Node[i]));
    // }
    // for (auto it=mesh.NodesBegin();it!=mesh.NodesEnd();++it)
    //     std::cout << *it;

    // // Mesh<Node,Properties,Small_Displacement_Element,Point_Load_Condition> 
    // //                      mesh(&V_Node,Prop,E_V,C_V);

    
    
    return 0;
}