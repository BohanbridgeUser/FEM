#include "../include/define.h"
#include "../include/Geometry/Vector.h"
#include "../include/Geometry/Point.h"
#include "../include/Container/points_container.h"
#include "../include/Geometry/geometry_dimension.h"
#include "../include/Geometry/geometry.h"
#include "../include/Geometry/line.h"
#include "../include/Geometry/triangle.h"
#include "../include/Geometry/quadrilateral_2d_4.h"
#include "../include/Geometry/tetrahedron.h"
#include "../include/Geometry/hexahedron.h"
#include "../include/Container/geometry_container.h"
#include "../include/Geometry/geometry_data.h"
#include "../include/Quadrature/integration_point.h"
#include "../include/Quadrature/line_gauss_legendre_integration_points.h"
#include "../include/Quadrature/quadrature.h"
#include "../include/Quadrature/quadrilateral_gauss_legendre_integration_points.h"
#include <iostream>
#include <vector>
int main()
{
    using std::cout;
    using std::endl;

// @ Point Test {
    cout << "**********Point Test**********\n";
    Point<3> V1(1.0,2.0,3.0);
    Point<3> V2(1.0,2.0,3.0);
    Point<3> V3, V4;
    cout << V1 << V2;
    cout << V1 + V2;
    V4 = operator+(V1,V2);
    cout << V4;
    cout << V1 - V2;
    V4 = operator-(V1,V2);
    cout << V4;
    cout << V1 * 2;
    V3 = operator*(3,V2);
    cout << V3;
    Point<3> V5(3.0,4.0,5.0);
    Point<3> V6(10.0,11.0,12.5);
    Point<3> V7(20.0,30.0,40.0);
    Point<3> V8(15.0,30.0,60.0);
    cout << "Distance: " << dist(V1,V5) << endl;
// }

// @ Points_Container Test { 
    cout << "**********Points_Container Test**********\n";
    std::vector<Point<3> > vp;
    vp.push_back(V1);
    vp.push_back(V2);
    vp.push_back(V3);
    vp.push_back(V4);
    vp.push_back(V5);
    Points_Container<Point<3> > P_C(vp);
    for (int i=0;i<5;++i) cout << P_C.GetValue(i);
//}

// @ Geometry Test { 
    cout << "**********Geometry Test**********\n";
    Dimension<3> DimensionType;
    Geometry<Point<3>> G(P_C);
    for (int i=0;i<3;++i) cout << G.GetValue(i);
//}

// @ Line Test { 
    cout << "**********Line Test**********\n";
    Line<Point<3> > L1(P_C);
    Line<Point<3> > L2(P_C);
    Line<Point<3> > L3(P_C);
    cout << L1.GetType() << ' ' <<L1.GetID() << ' ' << L1.GetPoint1().GetID() << ' ' << L1.GetPoint2().GetID() << endl;
    cout << L2.GetType() << ' '<< L2.GetID() << ' ' << L2.GetPoint1().GetID() << ' ' << L2.GetPoint2().GetID() << endl;
    cout << L3.GetType() << ' '<< L3.GetID() << ' ' << L3.GetPoint1().GetID() << ' ' << L3.GetPoint2().GetID() << endl;
//}

// @ Triangle Test { 
    cout << "**********Triangle Test**********\n";
    Triangle<Point<3> > Tri1(P_C);
    Triangle<Point<3> > Tri2(P_C);
    Triangle<Point<3> > Tri3(P_C);
    cout << Tri1.GetType() << ' ' <<Tri1.GetID() << endl;
    cout << Tri2.GetType() << ' '<< Tri2.GetID() << endl;
    cout << Tri3.GetType() << ' '<< Tri3.GetID() << endl;
//}

// @ Quadrangle Test { 
    cout << "**********Quadrilateral Test**********\n";
    Quadrilateral_2d_4<Point<3> > Qua1(P_C);
    Quadrilateral_2d_4<Point<3> > Qua2(P_C);
    Quadrilateral_2d_4<Point<3> > Qua3(P_C);
    cout << Qua1.GetType() << ' '<< Qua1.GetID() << endl;
    cout << Qua2.GetType() << ' '<< Qua2.GetID() << endl;
    cout << Qua3.GetType() << ' '<< Qua3.GetID() << endl;
//}

// @ Tetrahedron Test { 
    cout << "**********Tetrahedron Test**********\n";
    Tetrahedron<Point<3> > Tet1(P_C);
    Tetrahedron<Point<3> > Tet2(P_C);
    Tetrahedron<Point<3> > Tet3(P_C);
    cout << Tet1.GetType() << ' '<< Tet1.GetID() << endl;
    cout << Tet2.GetType() << ' '<< Tet2.GetID() << endl;
    cout << Tet3.GetType() << ' '<< Tet3.GetID() << endl;
//}

// @ Hexahedron Test { 
    cout << "**********Hexahedron Test**********\n";
    Hexahedron<Point<3> > Hex1(P_C);
    Hexahedron<Point<3> > Hex2(P_C);
    Hexahedron<Point<3> > Hex3(P_C);
    cout << Hex1.GetType() << ' '<< Hex1.GetID() << endl;
    cout << Hex2.GetType() << ' '<< Hex2.GetID() << endl;
    cout << Hex3.GetType() << ' '<< Hex3.GetID() << endl;
//}

// @ Geometry_Container Test { 
    cout << "**********Geometry_Constainer<Line> Test**********\n";
    Geometry_Container<Line<Point<3> > > L_C;
    L_C.insert(L1);
    L_C.insert(L2);
    L_C.insert(L3);
    for (int i=1;i<4;++i) {
        cout << L_C.at(i).GetID() << endl;
    }
    cout << L_C.size()<< endl;

    cout << "**********Geometry_Constainer<Triangle> Test**********\n";
    Geometry_Container<Triangle<Point<3> > > T_C;
    T_C.insert(Tri1);
    T_C.insert(Tri2);
    T_C.insert(Tri3);
    for (int i=1;i<4;++i) {
        cout << T_C.at(i).GetID() << endl;
    }
    cout << T_C.size()<< endl;

    cout << "**********Geometry_Constainer<Quadrangle> Test**********\n";
    Geometry_Container<Quadrilateral_2d_4<Point<3> > > Q_C;
    Q_C.insert(Qua1);
    Q_C.insert(Qua2);
    Q_C.insert(Qua3);
    for (int i=1;i<4;++i) {
        cout << Q_C.at(i).GetID() << endl;
    }
    cout << Q_C.size()<< endl;

    cout << "**********Geometry_Constainer<Tetrahedron> Test**********\n";
    Geometry_Container<Tetrahedron<Point<3> > > Tth_C;
    Tth_C.insert(Tet1);
    Tth_C.insert(Tet2);
    Tth_C.insert(Tet3);
    for (int i=1;i<4;++i) {
        cout << Tth_C.at(i).GetID() << endl;
    }
    cout << Tth_C.size()<< endl;

    cout << "**********Geometry_Constainer<Hexahedron> Test**********\n";
    Geometry_Container<Hexahedron<Point<3> > > Hex_C;
    Hex_C.insert(Hex1);
    Hex_C.insert(Hex2);
    Hex_C.insert(Hex3);
    for (int i=1;i<4;++i) {
        cout << Hex_C.at(i).GetID() << endl;
    }
    cout << Hex_C.size()<< endl;

    cout << "**********Jacobian Test**********\n";
    Point<3> QP1(3.0,0.0,0.0);
    Point<3> QP2(6.0,0.0,0.0);
    Point<3> QP3(6.0,3.0,0.0);
    Point<3> QP4(3.0,3.0,0.0);
    Points_Container<Point<3> > QP_C;
    QP_C.InsertPoint(QP1);
    QP_C.InsertPoint(QP2);
    QP_C.InsertPoint(QP3);
    QP_C.InsertPoint(QP4);
    Quadrilateral_2d_4<Point<3> > Q(QP_C);
    int IntegrationPointsNum = Quadrilateral_Gauss_Legendre_Integration_Points2::IntegrationPointsNumber();
    Quadrilateral_2d_4<Dimension<3> >::JacobiansContainer J_C(IntegrationPointsNum);
    Q.Jacobians(J_C,Geometry_Data::IntegrationMethod::Gauss_Legendre_2);
    for (int i=0;i<IntegrationPointsNum;++i) cout << J_C[0] << endl;
// }

    return 0;
}