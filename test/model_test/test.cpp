#include "../../include/define.h"
#include "../../include/Geometry/geometry.h"
#include "../../include/Geometry/Point.h"
#include "../../include/Geometry/quadrilateral_2d_4.h"
#include "../../include/element.h"
#include "../../include/properties.h"
#include "../../include/node.h"
#include "../../include/node_data.h"
#include "../../include/dof.h"
#include "../../include/condition.h"
#include "../../include/mesh.h"
#include "../../include/Model/model.h"
#include "../../include/Model/model_part.h"
int main()
{
    using std::cout;
    using std::endl;

///@brief Node Test {
    Point<3> P1(0.00,0.00,0.00),
             P2(1.00,0.00,0.00),
             P3(1.00,1.00,0.00),
             P4(0.00,1.00,0.00);

    cout << "************Node Test*************\n";
    Node N1(0.00,0.00,0.00),
         N2(1.00,0.00,0.00),
         N3(1.00,1.00,0.00),
         N4(0.00,1.00,0.00);
    Node N5(P1),N6(P2),N7(P3),N8(P4);
    cout << "Node Create:\n" << N1 << N2 << N3 << N4;
    cout << "Node Data:\n"
         << "N1 ID:" << N1.GetNodeData().GetID() << endl
         << "N2 ID:" << N2.GetNodeData().GetID() << endl
         << "N3 ID:" << N3.GetNodeData().GetID() << endl
         << "N4 ID:" << N4.GetNodeData().GetID() << endl
         << "N5 ID:" << N5.GetNodeData().GetID() << endl
         << "N6 ID:" << N6.GetNodeData().GetID() << endl
         << "N7 ID:" << N7.GetNodeData().GetID() << endl
         << "N8 ID:" << N8.GetNodeData().GetID() << endl;
    /* Supppose 6 dofs per node */
    Node_Data::SolutionOneStepNodalDataContainerType S_O_S_N_D(6,2.00);
    Node_Data::SolutionStepsNodalDataContainerType S_S_N_D;
    S_S_N_D.push_back(S_O_S_N_D);
    N1.GetNodeData().GetSolutionStepsNodalData() = S_S_N_D;
    for (int i=0;i<N1.GetNodeData().GetSolutionStepsNodalData().size();++i) {
        for (int j=0;j<N1.GetNodeData().GetSolutionStepsNodalData()[i].size();++j){
            cout << "Node1 Data Solution Step:"<< i 
                 << " Node1 Dof:" << j 
                 << "\nData:" << N1.GetNodeData().GetSolutionStepsNodalData()[i][j] << endl;
        }
    }
///@}

///@brief Element Test {
    cout << "**********Element Test**********\n";
    Points_Container<Node> N_C;
    N_C.InsertPoint(N5);
    N_C.InsertPoint(N6);
    N_C.InsertPoint(N7);
    N_C.InsertPoint(N8);
    Quadrilateral_2d_4<Node> Q(N_C);
    Properties::DataContainerType Elasticity(10,2.1e11);
    Properties::TableContainerType P_T_C{{std::string("Elasticity"),Elasticity}};
    Properties Property(P_T_C);
    Element E(&Q,Property);
    for (int i=0;i<E.GetGeometry().GetPointsNum();++i){
        cout << E.GetGeometry().pPointsVector().GetValue(i);
    }
///@}
    return 0;
}