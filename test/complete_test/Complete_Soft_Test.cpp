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
#include <memory>
int main()
{
    /* Geometry Model */
    std::cout << "********************Geometry Model********************\n";
    std::vector<Point<3>::Pointer> V_PP;
    for (int i=0;i<9;++i)
    {
        for (int j=0;j<9;++j)
        {
            for(int k=0;k<9;++k)
            {
                V_PP.push_back(std::make_shared<Point<3>>(1.00*i,1.00*j,1.00*k));
            }
        }
    }
    
    // for (auto iterator=V_PP.begin();iterator!=V_PP.end();++iterator)
    //     std::cout << "Point " << iterator-V_PP.begin() << " " << *iterator;
    
    std::vector<Hexahedron<Point<3>>::Pointer> H_3d_8s;
    int cnt = 0;
    for (int i=0;i<8;++i) 
    {
        for (int j=0;j<8;++j) 
        {
            for(int k=0;k<8;++k)
            {
                H_3d_8s.push_back(std::make_shared<Hexahedron<Point<3> >>(cnt,
                                                                      *V_PP[i*81+j*9+k],
                                                                      *V_PP[(i+1)*81+j*9+k],
                                                                      *V_PP[(i+1)*81+(j+1)*9+k],
                                                                      *V_PP[i*81+(j+1)*9+k],
                                                                      *V_PP[i*81+j*9+(k+1)],
                                                                      *V_PP[(i+1)*81+j*9+(k+1)],
                                                                      *V_PP[(i+1)*81+(j+1)*9+(k+1)],
                                                                      *V_PP[i*81+(j+1)*9+(k+1)]));
                cnt++;
            }
        }
    }
    /* Mesh Model */
    std::cout << "********************Mesh Model********************\n";
    Mesh<Node,Properties,Element,Condition> mesh;
    std::vector<Node*> V_P_N;
    std::cout << "Nodes Count: " << cnt << std::endl;
    for(int i=0;i<9*9*9;++i)
    {
        Node* N_P = new Node(i,*V_PP[i]);
        mesh.AddNode(Node::Pointer(N_P));
    }

    for(auto i=mesh.NodesBegin();i!=mesh.NodesEnd();++i)
    {
        std::cout << *i << std::endl;
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

    Hexahedron<Node>::Pointer H_3d_8Ns[cnt];
    cnt =0 ;
    for (int i=0;i<8;++i) 
    {
        for (int j=0;j<8;++j) 
        {
            for(int k=0;k<8;++k)
            {
                H_3d_8Ns[cnt] = std::make_shared<Hexahedron<Node>>(cnt,
                                               mesh.pGetNode(i*81+j*9+k)             ,
                                               mesh.pGetNode((i+1)*81+j*9+k)         ,
                                               mesh.pGetNode((i+1)*81+(j+1)*9+k)     ,
                                               mesh.pGetNode(i*81+(j+1)*9+k)         ,
                                               mesh.pGetNode(i*81+j*9+(k+1))         ,
                                               mesh.pGetNode((i+1)*81+j*9+(k+1))     ,
                                               mesh.pGetNode((i+1)*81+(j+1)*9+(k+1)) ,
                                               mesh.pGetNode(i*81+(j+1)*9+(k+1))    );
                cnt++;
            }
        }
    }
    for(int i=0;i<cnt;++i)
    {
        mesh.AddElement(std::make_shared<Small_Displacement_Element>(i,H_3d_8Ns[i],Properties::Pointer(&Prop)));
    }
    for(auto i=mesh.ElementsBegin();i!=mesh.ElementsEnd();++i)
    {
        std::cout << *i << std::endl;
    }

    for(int i=8;i<729;i+=9)
    {
        mesh.AddCondition(std::make_shared<Point_Load_Condition>(i,
                                                                std::make_shared<Point3D<Node>>(mesh.pGetNode(i)),
                                                                Properties::Pointer(&Prop)
                                                                ));
    }
    for(auto i=mesh.ConditionsBegin();i!=mesh.ConditionsEnd();++i)
    {
        std::cout << *i << std::endl;
    }
    
    Model Model1;
    Model1.CreateModelPart
    (
        std::string("Cube")
    );
    ModelPart& Cube = Model1.GetModelPart("Cube");
    return 0;
}