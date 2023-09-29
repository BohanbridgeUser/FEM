#include "../../include/lotus_kernel.h"

#include "../../include/process_info.h"
#include "../../include/Model/model.h"
#include "../../include/Element/small_displacement_element.h"
#include "../../include/Condition/point_load_condition.h"
#include "../../include/Variable/variable.h"
#include "../../include/Solve_Strategy/builder_and_solver/block_builder_and_solver.h"
#include "../../include/Solve_Strategy/strategies/linear_solving_strategy.h"
#include "../../include/Solve_Strategy/convergencecriterias/convergencecriterias.h"
#include "../../include/Solve_Strategy/schemes/scheme.h"
#include "../../include/Container/lotus_flags.h"


#include <memory>
int main()
{
    // /* Mesh Model */
    // std::cout << "********************Mesh Model********************\n";
    // Mesh<Node,Properties,Element,Condition> mesh;
    // std::vector<Node*> V_P_N;
    // std::cout << "Nodes Count: " << cnt << std::endl;
    // for(int i=0;i<9*9*9;++i)
    // {
    //     Node* N_P = new Node(i,*V_PP[i]);
    //     mesh.AddNode(Node::Pointer(N_P));
    // }

    // for(auto i=mesh.NodesBegin();i!=mesh.NodesEnd();++i)
    // {
    //     std::cout << *i << std::endl;
    // }
    
    // mesh.AddProperties(Properties::Pointer(&Prop));

    // Hexahedron<Node>::Pointer H_3d_8Ns[cnt];
    // cnt =0 ;
    // for (int i=0;i<8;++i) 
    // {
    //     for (int j=0;j<8;++j) 
    //     {
    //         for(int k=0;k<8;++k)
    //         {
    //             H_3d_8Ns[cnt] = std::make_shared<Hexahedron<Node>>(cnt,
    //                                            mesh.pGetNode(i*81+j*9+k)             ,
    //                                            mesh.pGetNode((i+1)*81+j*9+k)         ,
    //                                            mesh.pGetNode((i+1)*81+(j+1)*9+k)     ,
    //                                            mesh.pGetNode(i*81+(j+1)*9+k)         ,
    //                                            mesh.pGetNode(i*81+j*9+(k+1))         ,
    //                                            mesh.pGetNode((i+1)*81+j*9+(k+1))     ,
    //                                            mesh.pGetNode((i+1)*81+(j+1)*9+(k+1)) ,
    //                                            mesh.pGetNode(i*81+(j+1)*9+(k+1))    );
    //             cnt++;
    //         }
    //     }
    // }
    // for(int i=0;i<cnt;++i)
    // {
    //     mesh.AddElement(std::make_shared<Small_Displacement_Element>(i,H_3d_8Ns[i],Properties::Pointer(&Prop)));
    // }
    // for(auto i=mesh.ElementsBegin();i!=mesh.ElementsEnd();++i)
    // {
    //     std::cout << *i << std::endl;
    // }

    // for(int i=8;i<729;i+=9)
    // {
    //     mesh.AddCondition(std::make_shared<Point_Load_Condition>(i,
    //                                                             std::make_shared<Point3D<Node>>(mesh.pGetNode(i)),
    //                                                             Properties::Pointer(&Prop)
    //                                                             ));
    // }
    // for(auto i=mesh.ConditionsBegin();i!=mesh.ConditionsEnd();++i)
    // {
    //     std::cout << *i << std::endl;
    // }
    
    Lotus_Kernel Kernel;
    Variables_List V_L;
    V_L.AddDof(&DISPLACEMENT);
    V_L.AddDof(&ROTATION);
    V_L.Add(DISPLACEMENT);
    V_L.Add(ROTATION);
    V_L.Add(REACTION);
    V_L.Add(REACTION_MOMENT);
    Model Model1;
    Model1.CreateModelPart
    (
        std::string("Cube")
    );
    ModelPart& Cube = Model1.GetModelPart("Cube");
    /** Create Nodes **/
    unsigned int NodeCount = 0;
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            for(int k=0;k<9;++k)
            {
                Cube.CreateNewNode(NodeCount,1.00*i,1.00*j,1.00*k,std::make_shared<Variables_List>(V_L));
                NodeCount++;
            }
        }
    }
    // for(int i=0;i<NodeCount;++i)
    // {
    //     std::cout << Cube.GetNode(i);
    // }
    /** Create Geometry **/
    unsigned int GeometryCount = 0;
    for (int i=0;i<8;++i) 
    {
        for (int j=0;j<8;++j) 
        {
            for(int k=0;k<8;++k)
            {
                std::vector<ModelPart::IndexType> NodesId;
                NodesId.push_back(i*81+j*9+k);
                NodesId.push_back((i+1)*81+j*9+k);
                NodesId.push_back((i+1)*81+(j+1)*9+k);
                NodesId.push_back(i*81+(j+1)*9+k);
                NodesId.push_back(i*81+j*9+(k+1));
                NodesId.push_back((i+1)*81+j*9+(k+1));
                NodesId.push_back((i+1)*81+(j+1)*9+(k+1));
                NodesId.push_back(i*81+(j+1)*9+(k+1));
                Cube.CreateNewGeometry("Hexahedron",GeometryCount,NodesId);
                GeometryCount++;
            }
        }
    }
    // for(int i=0;i<GeometryCount;++i)
    // {
    //     std::cout << "Hexahedron " << i << "\n" << Cube.GetGeometry(i);
    // }

    /** Create Property **/
    Properties Prop;
    Prop.SetId(1);
    Prop.SetValue(DENSITY,7850.00);
    Prop.SetValue(ELASTICITY_MODULUS,2.1E9);
    Prop.SetValue(POISON,0.31);
    Cube.AddProperties(std::make_shared<Properties>(Prop));
    std::cout << "Property DENSITY           : " << Cube.GetProperties(1,0).GetValue(DENSITY) << std::endl;
    std::cout << "Property ELASTICITY_MODULUS: " << Cube.GetProperties(1,0).GetValue(ELASTICITY_MODULUS)<< std::endl;
    std::cout << "Property POISON            : " << Cube.GetProperties(1,0).GetValue(POISON)<< std::endl;
    
    /** Create Elments **/
    unsigned int ElementsCount = 0;
    for(int i=0;i<GeometryCount;++i)
    {
        Cube.CreateNewElement("Small_Displacement_Element",ElementsCount,Cube.pGetGeometry(i),Cube.pGetProperties(1));
        ElementsCount++;
    }
    return 0;
}