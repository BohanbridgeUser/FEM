#include "../../include/lotus_kernel.h"
#include "../../application/solid_mechanics/include/lotus_solid_mechanics_application.h"

#include "../../include/Linear_Solver/linear_solver.h"
#include "../../include/Solve_Strategy/strategies/linear_solving_strategy.h"
#include "../../include/Solve_Strategy/builder_and_solver/block_builder_and_solver.h"
#include "../../include/Solve_Strategy/schemes/static_scheme.h"
#include "../../include/Solve_Strategy/time_integration_methods/static_method.h"

#include "../../include/process_info.h"
#include "../../include/Model/model.h"
#include "../../include/Variable/variable.h"
#include "../../include/Solve_Strategy/builder_and_solver/block_builder_and_solver.h"
#include "../../include/Solve_Strategy/strategies/linear_solving_strategy.h"
#include "../../include/Solve_Strategy/convergencecriterias/convergencecriterias.h"
#include "../../include/Solve_Strategy/schemes/scheme.h"
#include "../../include/Container/lotus_flags.h"

#include <memory>
int main()
{
    Lotus_Kernel Kernel;
    Lotus_Solid_Mechanics_Application::Pointer pNewApplication = std::make_shared<Lotus_Solid_Mechanics_Application>();
    Kernel.ImportApplication(pNewApplication);
    Kernel.Initialize();

    Variables_List::Pointer V_L1 = std::make_shared<Variables_List>();
    V_L1->Add(DISPLACEMENT);
    V_L1->Add(REACTION);
    V_L1->Add(REACTION_MOMENT);
    V_L1->Add(EXTERNAL_FORCE);
    V_L1->Add(EXTERNAL_MOMENT);
    V_L1->Add(INTERNAL_FORCE);
    Model Model2;
    Model2.CreateModelPart(std::string("Cube2"));
    ModelPart& Cube2 = Model2.GetModelPart("Cube2");
    /** Create Nodes **/
    unsigned int NodeCount1 = 0;
    for(int i=0;i<3;++i)
    {
        for(int j=0;j<3;++j)
        {
            for(int k=0;k<3;++k)
            {
                Cube2.CreateNewNode(NodeCount1,1.00*i,1.00*j,1.00*k,V_L1);
                Cube2.GetNode(NodeCount1).AddDof(DISPLACEMENT_X,REACTION_X);
                Cube2.GetNode(NodeCount1).AddDof(DISPLACEMENT_Y,REACTION_Y);
                Cube2.GetNode(NodeCount1).AddDof(DISPLACEMENT_Z,REACTION_Z);
                NodeCount1++;
            }
        }
    }
    for(auto i=0;i<NodeCount1;++i)
    {
        std::cout << "Node :" << i << std::endl;
        std::cout << Cube2.GetNode(i);
    }
    /** Create Geometry **/
    unsigned int GeometryCount1 = 0;
    for (int i=0;i<2;++i) 
    {
        for (int j=0;j<2;++j) 
        {
            for(int k=0;k<2;++k)
            {
                std::vector<ModelPart::IndexType> NodesId;
                NodesId.push_back(i*9+j*3+k);
                NodesId.push_back((i+1)*9+j*3+k);
                NodesId.push_back((i+1)*9+(j+1)*3+k);
                NodesId.push_back(i*9+(j+1)*3+k);
                NodesId.push_back(i*9+j*3+(k+1));
                NodesId.push_back((i+1)*9+j*3+(k+1));
                NodesId.push_back((i+1)*9+(j+1)*3+(k+1));
                NodesId.push_back(i*9+(j+1)*3+(k+1));
                Cube2.CreateNewGeometry("Hexahedron",GeometryCount1,NodesId);
                GeometryCount1++;
            }
        }
    }
    /** Create Property **/
    Properties Prop1;
    Prop1.SetId(1);
    Prop1.SetValue(DENSITY,7850.00);
    Prop1.SetValue(ELASTICITY_MODULUS,2.6E11);
    Prop1.SetValue(POISON,0.31);
    Prop1.SetValue(YOUNG_MODULUS,2.6E11);
    Prop1.SetValue(POISSON_RATIO,0.31);
    Constitutive_Law const& r_clone_constitutive = Lotus_Components<Constitutive_Law>::Get("Linear_Elastic_3D_Law");

    Prop1.SetValue(CONSTITUTIVE_LAW,r_clone_constitutive.Clone());
    Cube2.AddProperties(std::make_shared<Properties>(Prop1));
    std::cout << "Property DENSITY           : " << Cube2.GetProperties(1,0).GetValue(DENSITY) << std::endl;
    std::cout << "Property ELASTICITY_MODULUS: " << Cube2.GetProperties(1,0).GetValue(ELASTICITY_MODULUS)<< std::endl;
    std::cout << "Property POISON            : " << Cube2.GetProperties(1,0).GetValue(POISON)<< std::endl;

    /** Create Elments **/
    unsigned int ElementsCount2 = 0;
    for(int i=0;i<GeometryCount1;++i)
    {
        Cube2.CreateNewElement("Small_Displacement_Element3D8N",++ElementsCount2,Cube2.pGetGeometry(i)->pPointsVector(),Cube2.pGetProperties(1));
    }

    /** Create Conditions **/
    unsigned int ConditionCount1 = 0;
    for(int i=2;i<NodeCount1;i+=3)
    {
        Geometry< Node >::PointsContainerType N_C;
        N_C.push_back(Cube2.pGetNode(i));
        Cube2.GetNode(i).SolutionStepData().GetVariablesList().Add(FORCE_RESIDUAL);
        Cube2.GetNode(i).SolutionStepData().GetVariablesList().Add(MOMENT_RESIDUAL);
        Cube2.CreateNewCondition("Point_Load_Condition3D1N",ConditionCount1,N_C,Cube2.pGetProperties(1));
        std::array<double, 3 > & PointLoad = Cube2.GetCondition(ConditionCount1).GetData().GetValue(FORCE_LOAD);
        PointLoad[2] = 1000000.00;
        ConditionCount1++;
    }

    /** Constrain Dofs **/
    for(int i=0;i<NodeCount1;i+=3)
    {
        Cube2.GetNode(i).pGetDof(DISPLACEMENT_X)->FixDof();
        Cube2.GetNode(i).pGetDof(DISPLACEMENT_Y)->FixDof();
        Cube2.GetNode(i).pGetDof(DISPLACEMENT_Z)->FixDof();
    }

    Process_Info P_I1;
    Cube2.SetProcessInfo(P_I1);
    Cube2.SetNodalSolutionStepVariablesList(V_L1);
    Cube2.SetBufferSize(2);

    typedef Static_Scheme<SparseSpace,DenseSpace>
                                                                StaticSchemeType;
    typedef Static_Method<Variable<std::array<double, 3>>,std::array<double, 3>>
                                                                StaticMethodType;
    StaticMethodType::Pointer pTIM1 = std::make_shared<StaticMethodType>(DISPLACEMENT);
    StaticSchemeType::IntegrationMethodsVectorType v_TIM1;
    v_TIM1.push_back(pTIM1);                                                            
    
    StaticSchemeType::Pointer S_P1 
    = std::make_shared<Static_Scheme<SparseSpace,DenseSpace> >(v_TIM1);
    typedef Linear_Solver<SparseSpace,DenseSpace>                    SolverType;
    SolverType::Pointer pSolver1 = std::make_shared<SolverType>();

    typedef Block_Builder_And_Solver<SparseSpace,
                             DenseSpace,
                             Linear_Solver<SparseSpace,DenseSpace> > BuilderAndSolverType;
    BuilderAndSolverType::Pointer B_S1 = std::make_shared<BuilderAndSolverType>(pSolver1);

    Flags SolverFlag1;
    SolverFlag1.Set(Solver_Local_Flags::COMPUTE_REACTIONS);
    Linear_Solving_Strategy<SparseSpace,
                            DenseSpace,
                            Linear_Solver<SparseSpace,DenseSpace> > solver1(Cube2,S_P1,B_S1,SolverFlag1);
    
    std::cout << "Check Solve Input : " << solver1.Check() << std::endl;

    solver1.SetEchoLevel(3);
    solver1.InitializeSolutionStep();
    solver1.SolveSolutionStep();
    solver1.FinalizeSolutionStep();

    // Model Model1;
    // Model1.CreateModelPart
    // (
    //     std::string("Cube")
    // );
    // ModelPart& Cube = Model1.GetModelPart("Cube");
    // /** Create Nodes **/
    // unsigned int NodeCount = 0;
    // for(int i=0;i<9;++i)
    // {
    //     for(int j=0;j<9;++j)
    //     {
    //         for(int k=0;k<9;++k)
    //         {
    //             Cube.CreateNewNode(NodeCount,1.00*i,1.00*j,1.00*k,V_L);
    //             Cube.GetNode(NodeCount).AddDof(DISPLACEMENT_X,REACTION_X);
    //             Cube.GetNode(NodeCount).AddDof(DISPLACEMENT_Y,REACTION_Y);
    //             Cube.GetNode(NodeCount).AddDof(DISPLACEMENT_Z,REACTION_Z);
    //             NodeCount++;
    //         }
    //     }
    // }
    // // for(int i=0;i<NodeCount;++i)
    // // {
    // //     std::cout << Cube.GetNode(i);
    // // }
    // /** Create Geometry **/
    // unsigned int GeometryCount = 0;
    // for (int i=0;i<8;++i) 
    // {
    //     for (int j=0;j<8;++j) 
    //     {
    //         for(int k=0;k<8;++k)
    //         {
    //             std::vector<ModelPart::IndexType> NodesId;
    //             NodesId.push_back(i*81+j*9+k);
    //             NodesId.push_back((i+1)*81+j*9+k);
    //             NodesId.push_back((i+1)*81+(j+1)*9+k);
    //             NodesId.push_back(i*81+(j+1)*9+k);
    //             NodesId.push_back(i*81+j*9+(k+1));
    //             NodesId.push_back((i+1)*81+j*9+(k+1));
    //             NodesId.push_back((i+1)*81+(j+1)*9+(k+1));
    //             NodesId.push_back(i*81+(j+1)*9+(k+1));
    //             Cube.CreateNewGeometry("Hexahedron",GeometryCount,NodesId);
    //             GeometryCount++;
    //         }
    //     }
    // }
    // // for(int i=0;i<GeometryCount;++i)
    // // {
    // //     std::cout << "Hexahedron " << i << "\n" << Cube.GetGeometry(i);
    // // }

    // /** Create Property **/
    // Properties Prop;
    // Prop.SetId(1);
    // Prop.SetValue(DENSITY,7850.00);
    // Prop.SetValue(ELASTICITY_MODULUS,2.1E9);
    // Prop.SetValue(POISON,0.31);
    // Prop.SetValue(YOUNG_MODULUS,2.1E9);
    // Prop.SetValue(POISSON_RATIO,0.31);
    // Constitutive_Law const& r_clone_constitutive = Lotus_Components<Constitutive_Law>::Get("Linear_Elastic_3D_Law");

    // Prop.SetValue(CONSTITUTIVE_LAW,r_clone_constitutive.Clone());
    // Cube.AddProperties(std::make_shared<Properties>(Prop));
    // std::cout << "Property DENSITY           : " << Cube.GetProperties(1,0).GetValue(DENSITY) << std::endl;
    // std::cout << "Property ELASTICITY_MODULUS: " << Cube.GetProperties(1,0).GetValue(ELASTICITY_MODULUS)<< std::endl;
    // std::cout << "Property POISON            : " << Cube.GetProperties(1,0).GetValue(POISON)<< std::endl;
    
    // /** Create Elments **/
    // unsigned int ElementsCount = 0;
    // for(int i=0;i<GeometryCount;++i)
    // {
    //     Cube.CreateNewElement("Small_Displacement_Element3D8N",++ElementsCount,Cube.pGetGeometry(i)->pPointsVector(),Cube.pGetProperties(1));
    // }
    // // for(int i=0;i<ElementsCount;++i)
    // // {
    // //     std::cout << "Element " << i << "\n"<< Cube.GetElement(i);
    // // }

    // /** Create Conditions **/
    // unsigned int ConditionCount = 0;
    // for(int i=8;i<729;i+=9)
    // {
    //     Geometry< Node >::PointsContainerType N_C;
    //     N_C.push_back(Cube.pGetNode(i));
    //     Cube.GetNode(i).SolutionStepData().GetVariablesList().Add(FORCE_RESIDUAL);
    //     Cube.GetNode(i).SolutionStepData().GetVariablesList().Add(MOMENT_RESIDUAL);
    //     Cube.CreateNewCondition("Point_Load_Condition3D1N",ConditionCount,N_C,Cube.pGetProperties(1));
    //     std::array<double, 3 > & PointLoad = Cube.GetCondition(ConditionCount).GetData().GetValue(FORCE_LOAD);
    //     PointLoad[2] = 100000.00;
    //     ConditionCount++;
    // }
    // // for(int i=0;i<ConditionCount;++i)
    // // {
    // //     std::cout << "Condition " << i << "\n" << Cube.GetCondition(i); 
    // // }

    // /** Constrain Dofs **/
    // for(int i=0;i<729;i+=9)
    // {
    //     Cube.GetNode(i).pGetDof(DISPLACEMENT_X)->FixDof();
    //     Cube.GetNode(i).pGetDof(DISPLACEMENT_Y)->FixDof();
    //     Cube.GetNode(i).pGetDof(DISPLACEMENT_Z)->FixDof();
    // }

    // Process_Info P_I;
    // Cube.SetProcessInfo(P_I);
    // Cube.SetNodalSolutionStepVariablesList(V_L);
    // Cube.SetBufferSize(2);
                                                            

    // typedef Static_Scheme<SparseSpace,DenseSpace>
    //                                                             StaticSchemeType;
    // typedef Static_Method<Variable<std::array<double, 3>>,std::array<double, 3>>
    //                                                             StaticMethodType;
    // StaticMethodType::Pointer pTIM = std::make_shared<StaticMethodType>(DISPLACEMENT);
    // StaticSchemeType::IntegrationMethodsVectorType v_TIM;
    // v_TIM.push_back(pTIM);                                                            
    
    // StaticSchemeType::Pointer S_P 
    // = std::make_shared<Static_Scheme<SparseSpace,DenseSpace> >(v_TIM);
    // typedef Linear_Solver<SparseSpace,DenseSpace>                    SolverType;
    // SolverType::Pointer pSolver = std::make_shared<SolverType>();

    // typedef Block_Builder_And_Solver<SparseSpace,
    //                          DenseSpace,
    //                          Linear_Solver<SparseSpace,DenseSpace> > BuilderAndSolverType;
    // BuilderAndSolverType::Pointer B_S = std::make_shared<BuilderAndSolverType>(pSolver);

    // Flags SolverFlag;
    // SolverFlag.Set(Solver_Local_Flags::COMPUTE_REACTIONS);
    // Linear_Solving_Strategy<SparseSpace,
    //                         DenseSpace,
    //                         Linear_Solver<SparseSpace,DenseSpace> > solver(Cube,S_P,B_S,SolverFlag);
    
    // std::cout << "Check Solve Input : " << solver.Check() << std::endl;

    // solver.SetEchoLevel(3);
    // solver.InitializeSolutionStep();
    // solver.SolveSolutionStep();
    // solver.FinalizeSolutionStep();


    return 0;
}