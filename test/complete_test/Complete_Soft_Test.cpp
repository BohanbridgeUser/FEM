#include "../../include/Model/model.h"
#include "../../include/Element/solid_element.h"
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
    Geometry_Container<Geomelem> G_C;
    for(int i=0;i<cnt;++i) 
    {
        G_C.Insert(*H_3d_8s[i]);

    }
    /* Mesh Model */
    std::cout << "********************Mesh Model********************\n";
    std::vector<Node> V_Node;
    for (int i=0;i<V_PP.size();++i)
    {
        Node* N = new Node(*V_PP[i]);
        V_Node.push_back(*N);
    }
    // for (int i=0;i<V_Node.size();++i)
    // {
    //     std::cout << *V_Node[i];
    // } 

    Hexahedron<Node >* H_3d_8Ns[cnt];
    cnt =0 ;
    for (int i=0;i<8;++i) 
    {
        for (int j=0;j<8;++j) 
        {
            for(int k=0;k<8;++k)
            {
                H_3d_8Ns[cnt] = new Hexahedron(cnt,V_Node[i*81+j*9+k],V_Node[(i+1)*81+j*9+k],V_Node[(i+1)*81+(j+1)*9+k],V_Node[i*81+(j+1)*9+k],
                                                V_Node[i*81+j*9+(k+1)],V_Node[(i+1)*81+j*9+(k+1)],V_Node[(i+1)*81+(j+1)*9+(k+1)],V_Node[i*81+(j+1)*9+(k+1)]);
                cnt++;
            }
        }
    }
    Properties Prop;
    Prop.Id() = 1;
    Prop.SetValue(DENSITY,7850.00);
    Prop.SetValue(ELASTICITY_MODULUS,2.1E9);
    Prop.SetValue(POISON,0.31);
    std::cout << "Property DENSITY           : " << Prop.GetValue(DENSITY) << std::endl;
    std::cout << "Property ELASTICITY_MODULUS: " << Prop.GetValue(ELASTICITY_MODULUS)<< std::endl;
    std::cout << "Property POISON            : " << Prop.GetValue(POISON)<< std::endl;

    std::vector<Small_Displacement_Element> E_V;
    for (int i=0;i<64;++i) 
    {
        E_V.push_back(Small_Displacement_Element(i,H_3d_8Ns[i],&Prop));
    }


    /******************************Condition*********************************/
    std::vector<Point3D<Node>> V_P3D;
    for(int i=8;i<729;i+=9)
    {
        Point3D<Node>* temp = new Point3D<Node>(&V_Node[i]);
        V_P3D.push_back(*temp);
    }
    for (auto it=V_P3D.begin();it!=V_P3D.end();++it)
        std::cout << *it;

    std::vector<Point_Load_Condition> C_V;
    for (auto it=V_P3D.begin();it!=V_P3D.end();++it)
    {
        C_V.push_back(Point_Load_Condition(it-V_P3D.begin(),&(*it),&Prop));
    }
    Mesh<Node,Properties,Small_Displacement_Element,Point_Load_Condition> 
                         mesh(&V_Node,Prop,E_V,C_V);

    
    return 0;
}