#include "../../include/Model/model.h"
#include "../../include/Element/solid_element.h"
#include "../../include/Variable/variable.h"
int main()
{
    /* Geometry Model */
    std::cout << "********************Geometry Model********************\n";
    Point<3> P[81] = 
    {
        {0.00,0.00,0.00},{1.00,0.00,0.00},{2.00,0.00,0.00},{3.00,0.00,0.00},{4.00,0.00,0.00},{5.00,0.00,0.00},{6.00,0.00,0.00},{7.00,0.00,0.00},{8.00,0.00,0.00},
        {0.00,1.00,0.00},{1.00,1.00,0.00},{2.00,1.00,0.00},{3.00,1.00,0.00},{4.00,1.00,0.00},{5.00,1.00,0.00},{6.00,1.00,0.00},{7.00,1.00,0.00},{8.00,1.00,0.00},
        {0.00,2.00,0.00},{1.00,2.00,0.00},{2.00,2.00,0.00},{3.00,2.00,0.00},{4.00,2.00,0.00},{5.00,2.00,0.00},{6.00,2.00,0.00},{7.00,2.00,0.00},{8.00,2.00,0.00},
        {0.00,3.00,0.00},{1.00,3.00,0.00},{2.00,3.00,0.00},{3.00,3.00,0.00},{4.00,3.00,0.00},{5.00,3.00,0.00},{6.00,3.00,0.00},{7.00,3.00,0.00},{8.00,3.00,0.00},
        {0.00,4.00,0.00},{1.00,4.00,0.00},{2.00,4.00,0.00},{3.00,4.00,0.00},{4.00,4.00,0.00},{5.00,4.00,0.00},{6.00,4.00,0.00},{7.00,4.00,0.00},{8.00,4.00,0.00},
        {0.00,5.00,0.00},{1.00,5.00,0.00},{2.00,5.00,0.00},{3.00,5.00,0.00},{4.00,5.00,0.00},{5.00,5.00,0.00},{6.00,5.00,0.00},{7.00,5.00,0.00},{8.00,5.00,0.00},
        {0.00,6.00,0.00},{1.00,6.00,0.00},{2.00,6.00,0.00},{3.00,6.00,0.00},{4.00,6.00,0.00},{5.00,6.00,0.00},{6.00,6.00,0.00},{7.00,6.00,0.00},{8.00,6.00,0.00},
        {0.00,7.00,0.00},{1.00,7.00,0.00},{2.00,7.00,0.00},{3.00,7.00,0.00},{4.00,7.00,0.00},{5.00,7.00,0.00},{6.00,7.00,0.00},{7.00,7.00,0.00},{8.00,7.00,0.00},
        {0.00,8.00,0.00},{1.00,8.00,0.00},{2.00,8.00,0.00},{3.00,8.00,0.00},{4.00,8.00,0.00},{5.00,8.00,0.00},{6.00,8.00,0.00},{7.00,8.00,0.00},{8.00,8.00,0.00},
    };
    Points_Container<Point<3> > P_C;
    for (auto p:P)
    {
        P_C.InsertPoint(p);
    }
    std::cout << "Number of Points: " << P_C.GetPointsNum() << std::endl;
    for (int i=0;i<81;++i) 
    {
        std::cout << i+1 << " " << P_C.GetValue(i);
    }
    
    Quadrilateral_3d_4<Point<3> > Q_3d_4s[64];
    for (int i=0;i<8;++i) 
    {
        for (int j=0;j<8;++j) 
        {
            Q_3d_4s[i*8+j] = Quadrilateral_3d_4<Point<3> >(P_C.GetValue(i*8+j),P_C.GetValue(i*8+j+1),P_C.GetValue((i+1)*8+j),P_C.GetValue((i+1)*8+j+1));
        }
    }
    typedef Quadrilateral_3d_4<Point<3> > geomelem;
    Geometry_Container<geomelem> G_C;
    for(int i=0;i<64;++i) G_C.Insert(Q_3d_4s[i]);
    

    /* Mesh Model */
    std::cout << "********************Mesh Model********************\n";
    Node Ns[81];
    for (int i=0;i<81;++i)
    {
        Ns[i] = Node(P[i]);
    }
    for (int i=0;i<81;++i)
    {
        std::cout << Ns[i];
    }    
    std::vector<Node> N_V;
    for (int i=0;i<81;++i)
    {
        N_V.push_back(Ns[i]);
    }
    for (int i=0;i<81;++i)
    {
        std::cout << N_V[i];
    }
    Quadrilateral_3d_4<Node > Q_3d_4Ns[64];
    for (int i=0;i<8;++i) 
    {
        for (int j=0;j<8;++j) 
        {
            Q_3d_4Ns[i*8+j] = Quadrilateral_3d_4<Node >(N_V[i*8+j],N_V[i*8+j+1],N_V[(i+1)*8+j],N_V[(i+1)*8+j+1]);
        }
    }
    Properties Prop;
    std::vector<double> V_E,V_M,V_P;
    V_E.push_back(2.1E9);
    Prop.InsertProperty("Elasticity",V_E);
    V_M.push_back(7850.00);
    Prop.InsertProperty("Mass",V_M);
    V_P.push_back(1.2E-5);
    Prop.InsertProperty("Poisson",V_P);
    std::cout << Prop;

    std::vector<Element> E_V;
    Element E[64];
    for (int i=0;i<64;++i) 
    {
        E_V.push_back(Element(i,&Q_3d_4Ns[i],Prop));
    }

    Condition Cond;
    std::vector<Condition> C_V;
    C_V.push_back(Cond);
    Mesh<Node,Properties,Element,Condition> 
                        mesh(&N_V,Prop,E_V,C_V);
    for(int i=0;i<mesh.Elements().size();++i)
    {
        std::cout << "Element ID: " << i+1 << std::endl;
        std::cout << mesh.Elements()[i];
    }
    std::cout << "Properties:\n" 
              << mesh.Property();
    return 0;
}